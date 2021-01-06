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

    //sending messages to pairs to begin transmission
    //send mesg to indx1 with index2 as payload
    MyMessage *msg = new MyMessage();
    msg->setM_Payload(std::to_string(index2).c_str());
    send(msg,"out",index1);

    //send mesg to index2 with index1 as payload
    MyMessage *msg1 = new MyMessage();
    msg1->setM_Payload(std::to_string(index1).c_str());
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
