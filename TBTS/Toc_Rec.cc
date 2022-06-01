/*
 * Toc_Rec.cc
 *
 *  Created on: Mar 12, 2021
 *  Author: Tushant & Jeni Athina
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * In the previous model we just created another packet if we needed to
 * retransmit. This is OK because the packet didn't contain much, but
 * in real life it's usually more practical to keep a copy of the original
 * packet so that we can re-send it without the need to build it again.
 */
class Toc_Rec : public cSimpleModule
{
  private:
   // simtime_t timeout;  // timeout
  //  cMessage *timeoutEvent;  // holds pointer to the timeout self-packet
  //  int seq;  // packet sequence number
  //  cMessage *packet;  // packet that has to be re-sent on timeout

  protected:
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Toc_Rec);


void Toc_Rec::handleMessage(cMessage *msg)
{
    EV << "Packet Received from Traffic Shapper \n";
    EV << "Received: " << msg->getName() << "\n";
    delete msg;
}

