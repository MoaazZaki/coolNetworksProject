/*  This module is responsible for assigning a new pair each 1 second
 *
 *
 */


#ifndef __COOLNETWORKSPROJECT_PARENT_H_
#define __COOLNETWORKSPROJECT_PARENT_H_

#include <omnetpp.h>
#include <fstream>
#include <string>
#include <iostream>

#include "MyMessage_m.h"
#include "fileReader.h"

#define MAX 50
#define FILES_PATH "input\\node"
using namespace omnetpp;




class Parent : public cSimpleModule
{
  protected:
    int n;
    bool isPaired[MAX];
    int filesCount;

    void scheduleNewPair();
    int generateRandomIndex();

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

};

#endif
