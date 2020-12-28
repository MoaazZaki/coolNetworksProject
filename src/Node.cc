//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"
Define_Module(Node);

void Node::createMessageEvent()
{
    if(nbuffered < par("MAX_SEQ").intValue())
    {
        scheduleAt(simTime() + par("NEW_MESSAGE_TIME").doubleValue(), newMessageEvent);
    }
}

void Node::inc(int&seq,int op)
{
    switch(op) // 0:next_frame_to_send , 1:ack_expected , 2:frame_expected
    {
        case 0:
            if(seq < par("MAX_SEQ").intValue())
                seq = (seq + 1 ) % (par("MAX_SEQ").intValue() + 1);
            if(seq + dynamic_window_start == buffer.size() )
            {
                ack_expected = 0;
                next_frame_to_send = 0;
                //frame_expected = 0;
                nbuffered = 0;
                dynamic_window_start = 0;
            }
            break;

        case 1:
            if(dynamic_window_start+par("MAX_SEQ").intValue() != buffer.size())
            {
                dynamic_window_start++;
                next_frame_to_send--;
            }
            seq = (seq + 1 ) % (par("MAX_SEQ").intValue() + 1);


            break;


        case 2:
            seq = (seq + 1 ) % (buffer.size());
            break;
    }

}

bool Node::between(int sf,int si,int sn) // Check if sf <= si < sn
{
    return (((sf <= si) && (si < sn)) || ((sn <sf) && (sf <= si)) || ((si < sn) && (sn < sf)));
}

MyMessage_Base* Node::createMessage(std::string inp) // Create new message given string
{
    //2) calculate the char count
    int size = inp.size() +2 ;
    //3) define the vector of the bitsets
    std::vector<std::bitset<8>> message;
    //4) append the charcount to the vector
    message.push_back(std::bitset<8>(size));
    //5) loop on the characters and append them to the vector
    for(int i = 0;i <inp.size();i++)    message.push_back(std::bitset<8>(inp[i]));
    //and calculate the paritty check simultaniouly
    std::bitset<8> evenParity (message[0]);
    for(int i = 1;i <message.size();i++) evenParity = evenParity ^ message[i];

//    int rand=uniform(0,1)*10;
//    if(rand<3)
//    {
//       int rand_index = uniform(0,1)*100 ;
//       rand_index = rand_index % inp.size();
//       inp[rand_index]=inp[rand_index]+5;
//    }

    MyMessage_Base *mmsg = new MyMessage_Base(inp.c_str());
    mmsg->setM_Payload(inp.c_str());
    mmsg->setM_Type(buffer.size()); // It's now a dummy variable so I'll use it in holding buffer size of the other side
    mmsg->setChar_Count(size);
    mmsg->setMycheckbits(evenParity);
    mmsg->setE_Type(FRAME_ARRIVAL);

    return mmsg;
}

void Node::sendNewMessage(int frame_nr,int frame_expected,std::vector<MyMessage_Base*> buffer)
{
    MyMessage_Base* message = buffer[frame_nr+dynamic_window_start]->dup();
    message->setSeq_Num(frame_nr+dynamic_window_start);
    message->setAck((frame_expected + par("MAX_SEQ").intValue() ) % (par("MAX_SEQ").intValue()  +1));
    send(message,"out");
    // TODO: START TIMER
    MyMessage_Base * currentTimeout = timeoutEvent->dup();
    currentTimeout->setAck(message->getAck());
    currentTimeout->setSeq_Num(frame_nr+dynamic_window_start);
    scheduleAt(simTime() + par("TIMEOUT").doubleValue(), currentTimeout);

}

void Node::initialize()
{
    ack_expected = 0;
    next_frame_to_send = 0;
    frame_expected = 0;
    nbuffered = 0;
    dynamic_window_start = 0;

    timeoutEvent = new MyMessage_Base("timeout");
    timeoutEvent->setE_Type(TIMEOUT);

    newMessageEvent = new MyMessage_Base("New Message");
    newMessageEvent->setE_Type(NETWORK_LAYER_READY);

    // creating random content for communication
    if ( strcmp(getName(),"Tic")==0)
    {
        buffer.push_back(createMessage("HI"));
        buffer.push_back(createMessage("How are you ?"));
        buffer.push_back(createMessage("It's great !"));
        buffer.push_back(createMessage("Can you hear me ?"));
        buffer.push_back(createMessage("yessss"));
        buffer.push_back(createMessage(":')"));
        buffer.push_back(createMessage("Can you kill me ?"));
        buffer.push_back(createMessage("I'm bored"));
        buffer.push_back(createMessage("Thanks"));
        buffer.push_back(createMessage("Sad"));
    }
    else{
        buffer.push_back(createMessage("HELLO"));
        buffer.push_back(createMessage("How is your day ?"));
        buffer.push_back(createMessage("I'm good"));
        buffer.push_back(createMessage("Yes !"));
        buffer.push_back(createMessage("Can you ?"));
        buffer.push_back(createMessage(":'D"));
        buffer.push_back(createMessage("Why?"));
        buffer.push_back(createMessage("okay sure"));
        buffer.push_back(createMessage("but I can't"));
        buffer.push_back(createMessage("yeah very sad"));
    }
    // Start scheduling
    createMessageEvent();
}

void Node::handleMessage(cMessage *msg)
{
    MyMessage_Base *mmsg = check_and_cast<MyMessage_Base *>(msg);

    switch(mmsg->getE_Type())
    {
        case NETWORK_LAYER_READY:
            nbuffered++;
            sendNewMessage(next_frame_to_send,frame_expected,buffer);
            inc(next_frame_to_send,0);

            createMessageEvent(); //schedule mew message to send
            break;

        case FRAME_ARRIVAL:
            if(mmsg->getSeq_Num() == frame_expected)
            {
                //Printing the cool details of the message
                bubble(mmsg->getM_Payload());
                EV<<"received message at: ";
                EV << getName();
                EV<<" received message with type: ";
                EV << mmsg->getM_Type();
                EV<<" received message with sequence number: ";
                EV << mmsg->getSeq_Num();
                EV<<" and payload of: ";
                EV<< mmsg->getM_Payload();
                EV<<" and ack of: ";
                EV<< mmsg->getAck();
                inc(frame_expected,2);
            }

            while(between(ack_expected,mmsg->getAck(),next_frame_to_send))
            {
                nbuffered--;
                //cancelEvent(timeoutEvent);
                inc(ack_expected,1);
            }
            break;

        case ERR:
            break; //Silent

        case TIMEOUT:
            if(mmsg->getSeq_Num() == ack_expected)
            {
                next_frame_to_send = ack_expected;
                for(int i=1; i<=nbuffered; i++)
                {
                    sendNewMessage(next_frame_to_send,frame_expected,buffer);
                    inc(next_frame_to_send,0);
                }
            }else
            {
                EV<< "Message with sequence number: ";
                EV<< mmsg->getSeq_Num();
                EV<< " delivered successfully :'D";
            }
            break;
    }








//    if ( strcmp(getName(),"Tic")==0)
//    {
//        if(mmsg->getM_Type() == 1) //Positive Ack
//            sendNewMessage(); //Send Message and increase seq number
//        else //Negate Ack
//        {
//            seq--; // Re-send message
//            sendNewMessage();
//        }
//    }
//    else
//    {
//        // TODO - Generated method body
//
//
//        std::vector<std::bitset<8>> message;
//
//        std::string inp = mmsg->getM_Payload();
//        for(int i = 0;i <inp.size();i++)    message.push_back(std::bitset<8>(inp[i]));
//
//        std::bitset<8> evenParity (message[0]);
//        for(int i = 1;i <inp.size();i++) evenParity = evenParity ^ message[i];
//
//        if(evenParity == mmsg->getMycheckbits()) //Correct
//        {
//            seq++;
//            MyMessage_Base *ackMessage = new MyMessage_Base("Ack");
//            ackMessage->setM_Type(1); //ACK
//            send(ackMessage,"out");
//        }
//        else
//        {
//            MyMessage_Base *ackMessage = new MyMessage_Base("nAck");
//            ackMessage->setM_Type(2); //NACK
//            send(ackMessage,"out");
//        }
//    }



}



