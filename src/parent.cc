/*  This module is responsible for assigning a new pair each 1 second
 *
 *
 */

#include "parent.h"

Define_Module(Parent);

void Parent::initialize()
{
    //Schedule a new pair
    n=par("numberofNodes");
    filesCount=par("fcount");
    memset(isPaired,n,false);
    scheduleNewPair();
}

int Parent::generateRandomIndex()
{
    //TODO::handle if all are used
    //int count=1;
    int index=uniform(0,1)*(n-1);
    while(isPaired[index])
         index=uniform(0,1)*(n-1);

    isPaired[index]=true;
    return index;
}

void Parent::scheduleNewPair()
{
    int index1=generateRandomIndex();
    int index2=generateRandomIndex();

    //getting a random file number for each peer
    fileReader fileR;
    int file1=uniform(0,1)*filesCount+1;
    std:: string filename1=FILES_PATH+std::to_string(file1)+".txt";
    int framesCount1=fileR.countLines(filename1.c_str());

    int file2=uniform(0,1)*filesCount+1;
    std:: string filename2=FILES_PATH+std::to_string(file2)+".txt";
    int framesCount2=fileR.countLines(filename2.c_str());

    //sending messages to pairs to begin transmission
    //send mesg to indx1 with index2 as payload
    MyMessage *msg = new MyMessage();
    msg->setSeq_Num(index2);
    msg->setM_Payload(filename1.c_str());
    msg->setReceived_Frames_Count(framesCount2);
    msg->setE_Type(CONTROL);
    send(msg,"out",index1);

    //send mesg to index2 with index1 as payload
    MyMessage *msg1 = new MyMessage();
    msg1->setSeq_Num(index1);
    msg1->setM_Payload(filename2.c_str());
    msg1->setReceived_Frames_Count(framesCount1);
    msg1->setE_Type(CONTROL);
    send(msg1,"out",index2);

    EV<<"Parent just made a pair ("<<index1<<","<<index2<<")\n";
    //Trigger rescheduling a pair
    scheduleAt(simTime() + 100.0, new MyMessage(""));
}

void Parent::handleMessage(cMessage *msg)
{

    if(msg->isSelfMessage())
    {
        EV<<"Parent received self message\n";
        scheduleNewPair();
    }
    else
    {
        //Receive end of trasmission of a node(pair)
        EV<<"Parent received message from node\n";
        int index=msg->getArrivalGate()->getIndex();
        isPaired[index]=false;
    }
    cancelAndDelete(msg);
}
