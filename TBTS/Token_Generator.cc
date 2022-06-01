/*
 * Token_Generator.cc
 *
 *  Created on: Mar 26, 2021
 *      Author: tushu
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
/*
class Token_Generator : public cSimpleModule
{
  private:
    double Token_InterArrivalTime;
   // simtime_t timeout;  // timeout
  //  cMessage *timeoutEvent;  // holds pointer to the timeout self-packet
  //  int seq;  // packet sequence number
  //  cMessage *packet;  // packet that has to be re-sent on timeout

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Token_Generator);

void Token_Generator::initialize()
{

    EV << "Receiving initial parameters\n";
    int token_rate = par("token_rate");
    Token_InterArrivalTime = 1/token_rate;
    message = generateNewMessage();

    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");
    sendCopyOf(message);
    scheduleAt(simTime()+timeout, timeoutEvent)
}

void Token_Generator::handleMessage(cMessage *msg)
{

    EV << "Packet Received from Traffic Shaper \n";
    EV << "Received: " << msg->getName() << "\n";

    if(msg->getName() == "Token_Released")
    {
    cMessage *msg = new cMessage("Token");
    sendDelayed(msg,Token_InterArrivalTime,"out");
    }

}

*/

class Token_Generator : public cSimpleModule
{
  private:
    int Tokens_Available;
    int Tokens_Released;
    simtime_t Token_InterArrivalTime;
    simtime_t timeout;  // timeout
    cMessage *timeoutEvent;  // holds pointer to the timeout self-message
    int seq;  // message sequence number
    cMessage *message;  // message that has to be re-sent on timeout
    cMessage *Token_Msg; //For sending token message

  public:
    Token_Generator();
    virtual ~Token_Generator();

  protected:
    //virtual cMessage *generateNewMessage();
    virtual void sendto(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Token_Generator);

Token_Generator::Token_Generator()
{
    timeoutEvent = message = nullptr;
}

Token_Generator::~Token_Generator()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Token_Generator::initialize()
{
    // Initialize variables.
    EV << "Receiving initial token_rate parameter in \n";

    int size_token_bucket = par("size_token_bucket");
    Tokens_Available=size_token_bucket;

    int token_rate = par("token_rate");
    Token_InterArrivalTime = (1.0/(simtime_t)token_rate);
    timeout = Token_InterArrivalTime;
    EV << "Token Generator timeout: " << timeout << "\n";
    Tokens_Released=0;
    timeoutEvent = new cMessage("timeoutEvent");

    //timeout = Token_InterArrivalTime;
    // Generate and send initial message.
    EV << "Sending initial message\n";
    message = new cMessage("Token");
    sendto(message);
    scheduleAt(simTime()+timeout, timeoutEvent);
}

void Token_Generator::handleMessage(cMessage *msg)
{
    if (msg == timeoutEvent) {
        // If we receive the timeout event, that means the packet hasn't
        // arrived in time and we have to re-send it.
        EV << "Timeout expired, resending message and restarting timer\n";
        if(Tokens_Available != 0)
            {
            Token_Msg = new cMessage("Token");
                sendto(Token_Msg);
                Tokens_Available--;
                EV << "Tokens Available Early bucket: " << Tokens_Available << "\n";
            }
        else if(Tokens_Available == 0 && Tokens_Released > 0)
        {
            Token_Msg = new cMessage("Token");
            sendto(Token_Msg);

            Tokens_Released--;
            EV << "Tokens Released: " << Tokens_Released << "\n";

        }
        scheduleAt(simTime()+timeout, timeoutEvent);
    }
    else if(strncmp(msg->getName(),"Token_Released",14) == 0){
        // message arrived
        // Token_Released received!
        EV << "Received: " << msg->getName() << "\n";
        delete msg;
        // Also delete the stored message and cancel the timeout event.
/*        EV << "Timer cancelled.\n";
        cancelEvent(timeoutEvent);
        delete message;*/
        Tokens_Released++;


       // scheduleAt(simTime()+timeout, timeoutEvent);
    }
}


void Token_Generator::sendto(cMessage *msg)
{
    EV << "Token Sent to Traffic shapper \n";
    EV << "Sent: " << msg->getName() << "\n";

    send(msg,"out");
}



