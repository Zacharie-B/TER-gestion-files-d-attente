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

#ifndef QUEUE_FIFOQUEUE_H_
#define QUEUE_FIFOQUEUE_H_

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;


class FifoQueue : public cSimpleModule {

private:
    intval_t frameCapacity;

    cQueue queue;
    cMessage *endTransmissionEvent = nullptr;
    bool isBusy;

    simsignal_t qlenSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;
    simsignal_t dropSignal;
    simsignal_t txBytesSignal;
    simsignal_t rxBytesSignal;

public:
    FifoQueue();
    virtual ~FifoQueue();

protected:
    virtual void initialize() override;
    virtual void startTransmitting(cMessage *msg);
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override;
};

#endif /* QUEUE_FIFOQUEUE_H_ */
