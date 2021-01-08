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

/*Resets data members of node in order to begin new transmission*/

void Node::reset()
{
        //Reseting Go-back-N variables

    ack_expected = 0;
    next_frame_to_send = 0;
    frame_expected = 0;
    nbuffered = 0;
    dynamic_window_start = 0;
    

    timeoutEvent = new MyMessage("timeout");
    timeoutEvent->setE_Type(TIMEOUT);

    newMessageEvent = new MyMessage("New Message");
    newMessageEvent->setE_Type(NETWORK_LAYER_READY);

    errorEvent = new MyMessage("Error");
    errorEvent->setE_Type(ERR);

    //Filling in messages Buffer to begin Transmission
    buffer.clear();
    std::vector<std::string> messages=fileReaderi.readFile(fileName);
    for(int i=0;i<messages.size();i++)
    {
        buffer.push_back(createMessage(messages[i]));
        EV<<"added message\n";
    }

    //appending buffer with empty frames if frames to receive are > frames to send
    for(int i=messages.size();i<framesCount;i++)
        buffer.push_back(createMessage(""));

    timeoutBuffer.resize(buffer.size());
}




void Node::createMessageEvent()
{
    if(nbuffered <= par("MAX_SEQ").intValue() && next_frame_to_send + dynamic_window_start != buffer.size())
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

            /*if(seq + dynamic_window_start == buffer.size() )
            {
                ack_expected = 0;
                next_frame_to_send = 0;
                //frame_expected = 0;
                nbuffered = 0;
                dynamic_window_start = 0;
            }*/
            break;

        case 1:
            /*if(dynamic_window_start+par("MAX_SEQ").intValue() != buffer.size())
            {
                dynamic_window_start++;
                next_frame_to_send--;
                //if(next_frame_to_send == 0)
                    //seq = (seq + 1 ) % (buffer.size());
            }
            else*/
            seq = (seq + 1 ) % (buffer.size());
            /*if(seq == par("MAX_SEQ").intValue() && par("MAX_SEQ").intValue() + dynamic_window_start == buffer.size())
            {
                ack_expected = 0;
                //next_frame_to_send = 0;
                //frame_expected = 0;
                //nbuffered = 0;
                //dynamic_window_start = 0;
            }*/

            break;


        case 2:
            seq = (seq + 1 ) % (buffer.size());
            break;
    }

}

void Node::moveDynamicWindow()
{
    while(ack_expected > 0 && dynamic_window_start+ par("MAX_SEQ").intValue() < buffer.size())
    {
        ack_expected--;
        next_frame_to_send--;
        dynamic_window_start++;
    }

}
bool Node::between(int sf,int si,int sn) // Check if sf <= si < sn
{
    return (((sf <= si) && (si < sn)) || ((sn <sf) && (sf <= si)) || ((si < sn) && (sn < sf)));
}

MyMessage* Node::createMessage(std::string inp) // Create new message given string
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



    MyMessage *mmsg = new MyMessage(inp.c_str());
    mmsg->setM_Payload(inp.c_str());
    //mmsg->setM_Type(buffer.size()); // It's now a dummy variable so I'll use it in holding buffer size of the other side
    mmsg->setChar_Count(size);
    mmsg->setMycheckbits(evenParity);
    mmsg->setE_Type(FRAME_ARRIVAL);

    return mmsg;
}

void Node::sendNewMessage(int frame_nr,int frame_expected,std::vector<MyMessage*> buffer)
{
    EV << "static "<< total_generated_frames << endl;
    EV<<"Node sending message\n";
    MyMessage* message = buffer[frame_nr+dynamic_window_start]->dup();
    message->setSeq_Num(frame_nr+dynamic_window_start);
    message->setAck((frame_expected +buffer.size()) % (buffer.size() +1));
    //message->setSeq_Num(0);
    //Shadow message for duplication
    MyMessage* shadow_message = buffer[frame_nr+dynamic_window_start]->dup();
    shadow_message->setSeq_Num(frame_nr+dynamic_window_start);
    shadow_message->setAck((frame_expected +buffer.size()) % (buffer.size() +1));


    //flags for errors
    bool loss = false;
    bool duplicate = false;
    bool delay = false;
    bool corruption = false;

    /*
     * //parameters for errors
    double delay_amount = 0.2;
    //limit (percentage of errors) //would be changed from the ini files
    double corr_limit = 3;
    double delay_limit = 3;
    double loss_limit = 3;
    double dup_limit = 3;
     *
    */
    //parameters for errors
    double delay_amount = par("delay_amount").doubleValue();
    //limit (percentage of errors) //would be changed from the ini files
    double corr_limit = par("corr_limit").doubleValue();
    double delay_limit = par("delay_limit").doubleValue();
    double loss_limit = par("loss_limit").doubleValue();
    double dup_limit = par("dup_limit").doubleValue();

    //Debug
    /*
    EV<< " delay_amount "<< std::to_string(delay_amount)<<endl;
    EV<< " delay_limit "<< std::to_string(delay_limit)<<endl;
    EV<< " corr_limit "<< std::to_string(corr_limit)<<endl;
    EV<< " dup_limit "<< std::to_string(dup_limit)<<endl;
    EV<< " loss_limit "<< std::to_string(loss_limit)<<endl;
    */
    //

    // Corrupt or not
    int rand=uniform(0,1)*10;
    EV<< " Rand is (corruption) "<< std::to_string(rand)<<endl;
    if(rand < corr_limit) //value will be parameterized
    {
        corruption = true;
        EV<< getName()<<" has an ERROR (Corruption)!"<<endl;
    }

    //Send or drop
    rand=uniform(0,1)*10;
    EV<< " Rand is (Drop) "<< std::to_string(rand)<<endl;
    if(rand < loss_limit ) //value will be parameterized
    {

        EV<< getName()<<" has an ERROR (Drop)!"<<endl;
        loss = true;
    }

    //Duplicate or not (separately i.e the probability will just send one additional packet or not)
    rand=uniform(0,1)*10;
    EV<< " Rand is (Duplication) "<< std::to_string(rand)<<endl;
    if(rand < dup_limit) //value will be parameterized
    {

        EV<< getName()<<" has an ERROR (Duplication)!"<<endl;
        duplicate = true;
    }

    rand=uniform(0,1)*10;
    EV<< " Rand is (Delay) "<< std::to_string(rand)<<endl;
    if(rand < delay_limit) //value will be parameterized
    {

        EV<< getName()<<" has an ERROR (delay)!"<<endl;
        delay = true;
    }

    if(!loss && !duplicate && !delay && !corruption) //1. Normal case (Error free)
    {
        send(message,"out",currentPeerIndex);
    }
    else if(!loss && !duplicate && !delay && corruption) //2.Corruption only
    {
       std::string inp = message->getM_Payload();
       if(inp != ""){
       //EV<< getName()<<" has an ERROR (Corruption)!"<<endl;
       int rand_index = uniform(0,1)*100 ;
       rand_index = rand_index % inp.size();
       inp[rand_index]=inp[rand_index]+5;
       message->setM_Payload(inp.c_str());
       }
       send(message,"out",currentPeerIndex);
    }
    else if(!loss && !duplicate && delay && !corruption) //3. Delay only
    {
        sendDelayed(message, delay_amount, "out",currentPeerIndex);
    }
    else if(!loss && !duplicate && delay && corruption) //4.Corruption + delay
    {
        //Corruption
        std::string inp = message->getM_Payload();
        if(inp != ""){
       //EV<< getName()<<" has an ERROR (Corruption)!"<<endl;
        int rand_index = uniform(0,1)*100 ;

        rand_index = rand_index % inp.size();
        inp[rand_index]=inp[rand_index]+5;
        message->setM_Payload(inp.c_str());
        }
        //Delay
        sendDelayed(message, delay_amount, "out",currentPeerIndex);
    }
    else if(!loss && duplicate && !delay && !corruption) //5.Duplication only
    {
        send(message,"out",currentPeerIndex);
        send(shadow_message,"out",currentPeerIndex);
    }
    else if(!loss && duplicate && !delay && corruption) //6. Duplication + corruption
    {
        //Corruption
        std::string inp = message->getM_Payload();
        if(inp != ""){
        int rand_index = uniform(0,1)*100 ;

        rand_index = rand_index % inp.size();
        inp[rand_index]=inp[rand_index]+5;
        message->setM_Payload(inp.c_str());
        shadow_message->setM_Payload(inp.c_str());
        }
        send(message,"out",currentPeerIndex);
        //Duplication
        send(shadow_message,"out",currentPeerIndex);
    }
    else if(!loss && duplicate && delay && !corruption) //7. Duplication + delay (assuming constant delay)
    {
        sendDelayed(message, delay_amount, "out",currentPeerIndex);
        sendDelayed(shadow_message, delay_amount, "out",currentPeerIndex);
    }
    else if(!loss && duplicate && delay && corruption) //8. Corruption + delay + duplicate
    {
        //Corruption
        std::string inp = message->getM_Payload();
        if(inp != ""){
        int rand_index = uniform(0,1)*100 ;

        rand_index = rand_index % inp.size();
        inp[rand_index]=inp[rand_index]+5;
        message->setM_Payload(inp.c_str());
        shadow_message->setM_Payload(inp.c_str());
        }
        //Delay + duplication
        sendDelayed(message, delay_amount, "out",currentPeerIndex);
        sendDelayed(shadow_message, delay_amount, "out",currentPeerIndex);
    }
    else if(loss && !duplicate && !delay && !corruption) //9. Loss
    {
        //Nothing
    }
    else if(loss && !duplicate && !delay && corruption) //10. Corruption + Loss
    {
        //Nothing
    }
    else if(loss && !duplicate && delay && !corruption) //11. Delay + Loss
    {
        //Nothing
    }
    else if(loss && !duplicate && delay && corruption) //12. Corruption + Delay + Loss
    {
        //Nothing
    }
    else if(loss && duplicate && !delay && !corruption) //13. Duplication + loss (Loss dominates i.e both are lost)
    {
        //Nothing
    }
    else if(loss && duplicate && !delay && corruption) //14. Corruption + duplication + loss
    {
        //Nothing
    }
    else if(loss && duplicate && delay && !corruption) //15. Delay + duplication + loss
    {
        //Nothing
    }
    else if(loss && duplicate && delay && corruption) //16. All kind of errors
    {
        //Nothing
    }

      //START TIMER
      MyMessage * currentTimeout = timeoutEvent->dup();
      currentTimeout->setAck(message->getAck());
      currentTimeout->setSeq_Num(frame_nr+dynamic_window_start);
      currentTimeout->setChar_Count(0);
      scheduleAt(simTime() + par("TIMEOUT").doubleValue(), currentTimeout);
      timeoutBuffer[frame_nr+dynamic_window_start] = currentTimeout;

}

void Node::ResendMessage(int frame_nr,int frame_expected,std::vector<MyMessage*> buffer)
{
    MyMessage* message = buffer[frame_nr+dynamic_window_start]->dup();
    message->setSeq_Num(frame_nr+dynamic_window_start);
    message->setAck((frame_expected +buffer.size()) % (buffer.size() +1));

    //stats
    total_retransmitted_frames ++;

    send(message,"out",currentPeerIndex);

    //ASSUME error free on re-sending any message
//    MyMessage * currentTimeout = timeoutEvent->dup();
//    currentTimeout->setAck(message->getAck());
//    currentTimeout->setSeq_Num(frame_nr+dynamic_window_start);
//    scheduleAt(simTime() + par("TIMEOUT").doubleValue(), currentTimeout);

}

void Node::cencelTimeout(int ack)
{
    if(timeoutBuffer[ack] == nullptr) return;
    //Send Cancel timeout event
   /* MyMessage * cancelTimeoutMessage = timeoutEvent->dup();
    cancelTimeoutMessage->setChar_Count(-1);
    cancelTimeoutMessage->setSeq_Num(mmsg->getSeq_Num());
    send(cancelTimeoutMessage,"out");*/
   cancelEvent(timeoutBuffer[ack]);
   delete timeoutBuffer[ack];
   timeoutBuffer[ack] = nullptr;
}

void Node::initialize()
{
    total_generated_frames = 0;
    total_dropped_frames = 0; //Dropped at sender or receiver?
    total_retransmitted_frames = 0;
    useful_data = 0;
    total_Acks = 0;
    total_Nacks = 0;
    //reset();
    n=getParentModule()->par("numberofNodes");
    currentPeerIndex=-1;
}

void Node::handleMessage(cMessage *msg)
{
    int index = n+1;
    MyMessage *mmsg = check_and_cast<MyMessage *>(msg);
    if(!mmsg->isSelfMessage())
    {
        index=msg->getArrivalGate()->getIndex();
        EV<<"Received message from "<<index<<endl;
    }
    //Message is from parent to initialize sending
    if(index==n-1)
    {
        //set pear index
        framesCount=mmsg->getReceived_Frames_Count();
        fileName= mmsg->getM_Payload();
        currentPeerIndex=mmsg->getSeq_Num();
        if(currentPeerIndex>getIndex())
            currentPeerIndex--;

        EV<<"Node received initialization message from parent, peer index is "<<mmsg->getSeq_Num()<<" "<<currentPeerIndex<<endl;
        EV<<"file path: "<<fileName<<"\n";
        reset();
        // Start scheduling
        createMessageEvent();
    }
    //Message is from pear
    else
    {
        //receive message from peer
        receiveMessageFromPeer(mmsg);
    }

}

void Node::receiveMessageFromPeer(MyMessage *mmsg)
{
        switch(mmsg->getE_Type())
        {
            case NETWORK_LAYER_READY:

                nbuffered++;
                sendNewMessage(next_frame_to_send,frame_expected,buffer);
                inc(next_frame_to_send,0);

                createMessageEvent(); //schedule mew message to send
                break;

            case FRAME_ARRIVAL:
            {
                if(mmsg->getSeq_Num() == frame_expected) //Condition modified for duplication case
                {
                    // Checking message
                    std::vector<std::bitset<8>> message;
                    message.push_back((int)mmsg->getChar_Count());
                    std::string inp = mmsg->getM_Payload();
                    for(int i = 0;i <inp.size();i++)    message.push_back(std::bitset<8>(inp[i]));

                    std::bitset<8> evenParity (message[0]);
                    for(int i = 1;i <message.size();i++) evenParity = evenParity ^ message[i];

                    static bool correctMessage = true;
                    if(evenParity != mmsg->getMycheckbits())
                    {
                        //Error detected
                        EV<< getName()<<":Message with seq "<<mmsg->getSeq_Num()<<" has an ERROR!"<<endl;
                        correctMessage = false;
                    }
                    if(correctMessage){
                        //Printing the cool details of the message
                        bubble(mmsg->getM_Payload());
                        EV<<"received message at: ";
                        EV << getName();
                        EV<<" received message with type: ";
                        EV << mmsg->getReceived_Frames_Count();
                        EV<<" received message with sequence number: ";
                        EV << mmsg->getSeq_Num();
                        EV<<" and payload of: ";
                        EV<< mmsg->getM_Payload();
                        EV<<" and ack of: ";
                        EV<< mmsg->getAck();



                        /*while(between(ack_expected+dynamic_window_start,mmsg->getAck(),next_frame_to_send+dynamic_window_start))
                        {
                            nbuffered--;
                            cencelTimeout(ack_expected+dynamic_window_start);
                            inc(ack_expected,1);
                        }*/

                        if(mmsg->getSeq_Num() == frame_expected)
                            inc(frame_expected,2);


                        /*if(nbuffered == 0)
                        {
                            next_frame_to_send = 0;
                            dynamic_window_start =0;
                        }*/
                    }
                    else
                        correctMessage = true;

                }
                else
                {
                    EV<<"received message at: ";
                    EV << getName();
                    EV<<" with seq= ";
                    EV << mmsg->getSeq_Num();
                    EV<<" BUT IGNORED!!";
                }

                while(between(ack_expected+dynamic_window_start,mmsg->getAck(),next_frame_to_send+dynamic_window_start))
                {
                    nbuffered--;
                    cencelTimeout(ack_expected+dynamic_window_start);
                    inc(ack_expected,1);
                }
                moveDynamicWindow();

                if(ack_expected+dynamic_window_start == buffer.size()-1 && nbuffered !=0)
                {
                    nbuffered= 0;
                    for(int i = 0; i < timeoutBuffer.size();i++)
                    {
                        if(timeoutBuffer[i] != nullptr)
                        {
                            cancelAndDelete(timeoutBuffer[i]);
                            timeoutBuffer[i]=nullptr;
                        }
                    }
                    ack_expected = 0;

                    //TODO::send to parent
                    MyMessage* parentMsg=new MyMessage(" ");
                    send(parentMsg,"out",n-1);
                }

                delete mmsg;
                break;
            }
            case ERR:
                break; //Silence

            case TIMEOUT:

                next_frame_to_send = ack_expected;
                EV<< getName()<<": Resending starting from: ";
                EV<< next_frame_to_send + dynamic_window_start<<endl;
                for(int i=1; i<=nbuffered; i++)
                {

                    ResendMessage(next_frame_to_send,frame_expected,buffer);
                    EV<< "Message with seq "<<next_frame_to_send + dynamic_window_start<<" has been re-sent"<<endl;
                    inc(next_frame_to_send,0);
                }
                //next_frame_to_send -= dynamic_window_start; //Move window forward

                /*cFutureEventSet * queue = cSimulation::getActiveSimulation()->getFES();

                for (int i =0; i< queue->getLength();i++)
                {
                    cEvent* event = queue->get(i);

                    MyMessage *eventMsg = check_and_cast<MyMessage *>(event);
                    if(eventMsg->isSelfMessage() && eventMsg->getM_Type() == TIMEOUT)
                    {
                        cancelAndDelete(eventMsg);
                    }
                }*/
                for(int i = 0; i < timeoutBuffer.size();i++)
                {
                    if(timeoutBuffer[i] != nullptr)
                    {
                        cancelAndDelete(timeoutBuffer[i]);
                        timeoutBuffer[i]=nullptr;
                    }
                }

                //delete mmsg;

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
    //            MyMessage *ackMessage = new MyMessage("Ack");
    //            ackMessage->setM_Type(1); //ACK
    //            send(ackMessage,"out");
    //        }
    //        else
    //        {
    //            MyMessage *ackMessage = new MyMessage("nAck");
    //            ackMessage->setM_Type(2); //NACK
    //            send(ackMessage,"out");
    //        }
    //    }

        /*if (cSimulation::getActiveSimulation()->getFES()->getLength() == 0)
        {
            MyMessage* message = new MyMessage("Restart");
            message->setSeq_Num(-1);
            send(message,"out");

            initialize();
        }*/
}

void Node::finish()
{
/*
    ack_expected = 0;
    next_frame_to_send = 0;
    frame_expected = 0;
    nbuffered = 0;
    dynamic_window_start = 0;
    createMessageEvent();
*/

}

