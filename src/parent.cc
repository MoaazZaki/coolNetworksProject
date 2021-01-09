/*  This module is responsible for assigning a new pair each 1 second
 *
 *
 */

#include "parent.h"
#include "Node.h"
Define_Module(Parent);

/* Intializes parent by setting necessary vars & scheduling a new pair*/
void Parent::initialize()
{

    //Schedule a new pair
    n=par("numberofNodes");
    filesCount=par("fcount");
    memset(isPaired,n,false);
    scheduleNewPair();
    scheduleAt(simTime() + 7, new MyMessage(""));
}

/*Generates a random index for a pair in condition it's not used before*/
int Parent::generateRandomIndex()
{
    //TODO::handle if all are used
    //int count=1;
    int index=uniform(0,1)*(n-1);
    //Modified here (Ahmed)
    int dummy_counter = 0;
    while(isPaired[index] && dummy_counter < 100)
    {

         index=uniform(0,1)*(n-1);
         dummy_counter += 1;
    }
    if(isPaired[index])
        return -1;
    //////
    isPaired[index]=true;
    return index;
}

/*Schedules a new pair for transmission*/
void Parent::scheduleNewPair()
{
    int index1=generateRandomIndex();
    int index2=generateRandomIndex();

    //Modified here (Ahmad)
    if(index1 == -1 || index2 == -1)
        return;
    //

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
}

/*Handles message whether it's a self message or a message from a one of the nodes
 * to signal end of tramission.
 */
void Parent::handleMessage(cMessage *msg)
{

    if(msg->isSelfMessage())
    {
        //schedule a new pair with probability 50%
        EV<<"Parent received self message\n";
        int prob=uniform(0,1)*5;
        if(prob>2)
            scheduleNewPair();

        //Trigger rescheduling a pair
        scheduleAt(simTime() + 7, new MyMessage(""));
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
