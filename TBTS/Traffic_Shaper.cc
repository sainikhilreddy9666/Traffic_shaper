/*
 * Traffic_Shaper.cc
 *
 *  Created on: Mar 12, 2021
 *     Author: Tushant & Jeni Athina
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
class Traffic_Shaper : public cSimpleModule
{
private:
    int Register_Token_flag = 0;
    //int seq;  // packet sequence number
    //cMessage *packet;  // packet that has to be re-sent on timeout
    cQueue queue;
    int Token_Status;
    int Queue_capacity;

public:
    Traffic_Shaper();
  virtual ~Traffic_Shaper();

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
/*    cMessage *Token_Par_Added(cMessage *msg);
    cMessage *Token_Par_Removed(cMessage *msg);
    cMessage *Token_Register(cMessage *msg);
    cMessage *Token_Deregister(cMessage *msg);*/
    void Traffic_Shaper_Queue(cMessage *msg);
};

Define_Module(Traffic_Shaper);

Traffic_Shaper::Traffic_Shaper()
{
   // timeoutEvent = message = nullptr;
}

Traffic_Shaper::~Traffic_Shaper()
{
   // cancelAndDelete(timeoutEvent);
   // delete message;
}


void Traffic_Shaper::initialize()
{
    Queue_capacity = par("Queue_capacity");
    queue.setName("Traffic_Shaper_Queue");


}

void Traffic_Shaper::handleMessage(cMessage *msg)
{
        if(strncmp(msg->getName(),"Token",5)==0)
        {
            EV << "Received Token Packet\n";

            Register_Token_flag = 1;
            EV << "Register Tokens: " << Register_Token_flag <<endl;
            Traffic_Shaper_Queue(msg);
            delete msg;
        }
        else
        {
            EV << "Received packet from station A \n";
            cMessage *msgcopy = (cMessage *)msg->dup();
            EV << "Packet name: " << msgcopy->getName() <<endl;
            delete msg;
            Traffic_Shaper_Queue(msgcopy);

        }
}

void Traffic_Shaper::Traffic_Shaper_Queue(cMessage *msgtemp)
{
    if(strncmp(msgtemp->getName(),"tic-1",5)==0)
    {  // check for container capacity
        if (Queue_capacity >= 0 && queue.getLength() >= Queue_capacity) {
            EV << "Capacity full! Packet dropped.\n";
            if (hasGUI())
                bubble("Dropped!");
            delete msgtemp;
        }
        else
        {
            EV << "Queue Insertion started \n";
            //msgtemp=Token_Par_Added(msgtemp);
            EV << "Packet Inserted in Queue: " << msgtemp->getName() <<endl;
            queue.insert(msgtemp);
        }
    }
    if(Register_Token_flag == 1)
    {
        cMessage *msgtempout = (cMessage *)queue.pop();
        EV << "Packet Pop Out : " << msgtempout->getName() <<endl;

        // Sending Message to Token Generator that token is released
        cMessage *msg1 = new cMessage("Token_Released");
        send(msg1,"Token_out");
        Register_Token_flag = 0;

        // Sending the message forward to Station B(Toc_Rec) by making a copy
        cMessage *msgcopy = (cMessage *)msgtempout->dup();
        delete msgtempout;
        send(msgcopy,"out");

    }

}

/*
cMessage *Traffic_Shaper::Token_Par_Added(cMessage *msg)
{
    msg->addPar("Token_Status");
    msg->par("Token_Status") = 0;
    return msg;
}
cMessage *Traffic_Shaper::Token_Par_Removed(cMessage *msg)
{
    msg->removeObject("Token_Status");
    return msg;
}
cMessage *Traffic_Shaper::Token_Register(cMessage *msg)
{
    msg->par("Token_Status") = 1;
    return msg;
}

cMessage *Traffic_Shaper::Token_Deregister(cMessage *msg)
{
    msg->par("Token_Status") = 0;
    return msg;
}
*/


