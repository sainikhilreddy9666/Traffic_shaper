/*
 * Tic_Trans.cc
 *
 *  Created on: Mar 12, 2021
 *      Author: Tushant & Jeni Athina
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
class Tic_Trans : public cSimpleModule
{
  private:
    simtime_t Inter_Arrival_Time_Source;
   simtime_t timeout;  // timeout
   cMessage *timeoutEvent;  // holds pointer to the timeout self-packet
    int seq=0;  // packet sequence number
    cMessage *message;  // packet that has to be re-sent on timeout
  public:
    Tic_Trans();
    virtual ~Tic_Trans();

  protected:
    virtual cMessage *generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Tic_Trans);

Tic_Trans::Tic_Trans()
{
    timeoutEvent = message = nullptr;
}

Tic_Trans::~Tic_Trans()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Tic_Trans::initialize()
{

    Inter_Arrival_Time_Source = poisson(7);
    message = generateNewMessage();

    timeout = Inter_Arrival_Time_Source/100;
    EV << "Station A timeout: " << timeout << "\n";
    timeoutEvent = new cMessage("timeoutEvent");
    sendCopyOf(message);
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Tic_Trans::handleMessage(cMessage *msg)
{

    if (msg == timeoutEvent) {
        Inter_Arrival_Time_Source = poisson(7);             // Poisson distribution applied
        timeout = Inter_Arrival_Time_Source/100;
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Station A timeout: " << timeout << "\n";
        EV << "Station A: Timeout expired, resending message and restarting timer\n";
        sendCopyOf(message);
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
}

cMessage *Tic_Trans::generateNewMessage()
{
    // Generate a packet with a different name every time.
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);
    cMessage *msg = new cMessage(msgname);
    return msg;
}
void Tic_Trans::sendCopyOf(cMessage *msg)
{
    // Duplicate message and send the copy.
    cMessage *copy = (cMessage *)msg->dup();
    send(copy, "out");
}
