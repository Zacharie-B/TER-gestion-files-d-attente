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

#include <stdio.h>
#include <string.h>

#include "omnetpp/csimplemodule.h"

#include "DropTailQueuePpp.h"


DropTailQueuePpp::~DropTailQueuePpp() {
    cSimpleModule::cancelAndDelete(endTransmissionEvent);
}

void DropTailQueuePpp::initialize(int stage)
{
    MacProtocolBase::initialize(stage);

    // all initialization is done in the first stage
    if (stage == INITSTAGE_LOCAL) {
        displayStringTextFormat = cSimpleModule::par("displayStringTextFormat");
        sendRawBytes = cSimpleModule::par("sendRawBytes");
        endTransmissionEvent = new cMessage("pppEndTxEvent");
        MacProtocolBase::lowerLayerInGateId = cSimpleModule::findGate("$i");
        physOutGate = cSimpleModule::gate("outGate");
        physInGate = cSimpleModule::gate("inGate");
        MacProtocolBase::lowerLayerOutGateId = physOutGate->getId();

        // we're connected if other end of connection path is an input gate
        bool connected = physInGate->getPathEndGate()->getType() == cGate::INPUT;
        // if we're connected, get the gate with transmission rate
        datarateChannel = connected ? physOutGate->getTransmissionChannel() : nullptr;

        numSent = numRcvdOK = numDroppedBitErr = numDroppedIfaceDown = 0;
        WATCH(numSent);
        WATCH(numRcvdOK);
        WATCH(numDroppedBitErr);
        WATCH(numDroppedIfaceDown);

        subscribe(POST_MODEL_CHANGE, this);
        emit(transmissionStateChangedSignal, 0L);

        txQueue = check_and_cast<queueing::IPacketQueue *>(getSubmodule("dropTailQueue"));
    }
}

