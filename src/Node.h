/*Node file*/

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


    //Statistics
    double total_generated_frames; //Total sent at the sender side
    double total_dropped_frames;    //Total dropped at the sender side
    double total_retransmitted_frames;  //Total frames retransmitted due to noise in channel
    double useful_data; //data received and accepted at receiver
    double total_Acks;  //Same as no. of generated frames (As it can be sent more than one time if message is retransmitted)
    double total_Nacks; //zero
    bool statsPrinted;

    //Necessary data members for transmission
    int n;
    int currentPeerIndex;
    std::string fileName;
    int framesCount;
    fileReader fileReaderi;
    bool firstTime = true;

    //Go back-N data members
    int next_frame_to_send; //si
    int ack_expected; //sf
    int frame_expected; // Last received ack
    int dynamic_window_start;

    //Messages buffers
    std::vector<MyMessage*> buffer;
    int nbuffered;

    std::vector<MyMessage*> timeoutBuffer;
    MyMessage*  timeoutEvent;
    MyMessage*  newMessageEvent;
    MyMessage*  errorEvent;
    MyMessage*  finishEvent;


  public:
    virtual void initialize();
    void reset();
    virtual void handleMessage(cMessage *msg);
    void printStats();

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

};

#endif
