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

#ifndef QUEUECOMPONENTS_DROPTAILQUEUEPPP_H_
#define QUEUECOMPONENTS_DROPTAILQUEUEPPP_H_

#include "inet/queueing/contract/IPacketQueue.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/base/MacProtocolBase.h"
#include "inet/linklayer/ppp/PppFrame_m.h"

using namespace omnetpp;
using namespace inet;

class DropTailQueuePpp : public MacProtocoleBase
{
protected:
    const char *displayStringTextFormat = nullptr;
    bool sendRawBytes = false;
    cGate *physOutGate = nullptr;
    cGate *physInGate = nullptr;
    cChannel *datarateChannel = nullptr;    // nullptr if we're not connected

    cMessage *endTransmissionEvent = nullptr;

    std::string oldConnColor;

    // statistics
    long numSent = 0;
    long numRcvdOK = 0;
    long numDroppedBitErr = 0;
    long numDroppedIfaceDown = 0;

    static simsignal_t transmissionStateChangedSignal;
    static simsignal_t rxPkOkSignal;

    virtual void startTransmitting();
    virtual void encapsulate(Packet *msg);
    virtual void decapsulate(Packet *packet);
    virtual void refreshDisplay() const override;
    virtual void refreshOutGateConnection(bool connected);

    // cListener function
    virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) override;

    // MacBase functions
    virtual void configureInterfaceEntry() override;

public:
    DropTailQueuePpp();
    virtual ~DropTailQueuePpp();

protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessageWhenUp(cMessage *message) override;
    virtual void handleSelfMessage(cMessage *message) override;
    virtual void handleUpperPacket(Packet *packet) override;
    virtual void handleLowerPacket(Packet *packet) override;
    virtual void handleStopOperation(LifecycleOperation *operation) override;
};

#endif /* QUEUECOMPONENTS_DROPTAILQUEUEPPP_H_ */
