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

#include "FifoQueue.h"

Define_Module(FifoQueue);

FifoQueue::FifoQueue() {
    // TODO Auto-generated constructor stub

}

FifoQueue::~FifoQueue() {
    // TODO Auto-generated destructor stub
    cancelAndDelete(endTransmissionEvent);
}

void FifoQueue::initialize(){
    queue.setName("fifoQueue");
    endTransmissionEvent = new cMessage("endTransmissionEvent");

   if (par("useCutThroughSwitching"))
       gate("line$i")->setDeliverImmediately(true);

   frameCapacity = par("frameCapacity");

   qlenSignal = registerSignal("qlen");
   busySignal = registerSignal("busy");
   queueingTimeSignal = registerSignal("queueingTime");
   dropSignal = registerSignal("drop");
   txBytesSignal = registerSignal("txBytes");
   rxBytesSignal = registerSignal("rxBytes");

   emit(qlenSignal, queue.getLength());
   emit(busySignal, false);
   isBusy = false;
}


