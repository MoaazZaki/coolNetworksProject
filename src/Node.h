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

#ifndef __SECTION1_NODE_H_
#define __SECTION1_NODE_H_

#include <omnetpp.h>
#include <bitset>
#include <stdio.h>
#include <vector>
#include "MyMessage_m.h"
#include "fileReader.h"

using namespace omnetpp;

enum eventType {NETWORK_LAYER_READY,FRAME_ARRIVAL,ERR,TIMEOUT};

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

    //
    /*
    double delay_amount = default(0.2);
    //limit (percentage of errors) //would be changed from the ini files
    int corr_limit = default(3);
    int delay_limit = default(3);
    int loss_limit = default(3);
    int dup_limit = default(3);
    */

    //

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
    virtual void initialize();
    void receiveMessageFromPeer(MyMessage *mmsg);
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

};

#endif
