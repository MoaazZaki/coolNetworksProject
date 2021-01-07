/*Node class*/

#ifndef __SECTION1_NODE_H_
#define __SECTION1_NODE_H_

#include <omnetpp.h>
#include <bitset>
#include <stdio.h>
#include <vector>
#include "MyMessage_m.h"
#include "fileReader.h"
using namespace omnetpp;

class Node : public cSimpleModule
{
    //Necessary data members for tranmission
    int n;
    int currentPeerIndex;
    std::string fileName;
    int framesCount;
    fileReader fileReaderi;

    //Go back-N data members
    int next_frame_to_send; //si
    int ack_expected; //sf
    int frame_expected; // Last revieced ack
    int dynamic_window_start;

    //Messages buffers
    std::vector<MyMessage*> buffer;
    int nbuffered;

    std::vector<MyMessage*> timeoutBuffer;
    MyMessage*  timeoutEvent;
    MyMessage*  newMessageEvent;
    MyMessage*  errorEvent;

  public:
    virtual void initialize();
    void reset();
    virtual void handleMessage(cMessage *msg);

    //Go-back-N methods
    void createMessageEvent();
    MyMessage* createMessage(std::string inp);
    void sendNewMessage(int frame_nr,int frame_expected,std::vector<MyMessage*> buffer);
    void ResendMessage(int frame_nr,int frame_expected,std::vector<MyMessage*> buffer);
    void cencelTimeout(int ack);
    void moveDynamicWindow();
    bool between(int sf,int si,int sn);
    void inc(int&seq,int op);
    void receiveMessageFromPeer(MyMessage *mmsg);
    virtual void finish();

};

#endif
