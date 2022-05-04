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

#include "RandomQueue.h"

Define_Module(RandomQueue);

RandomQueue::RandomQueue()
{
	// TODO Auto-generated constructor stub

}

RandomQueue::~RandomQueue()
{
	// TODO Auto-generated destructor stub
	cancelAndDelete(endTransmissionEvent);
}

void RandomQueue::initialize()
{
	queue.setName("randomQueue");
	srand(time(0));
	endTransmissionEvent = new cMessage("endTransmissionEvent");

	if (par("useCutThroughSwitching"))
		 gate("link$i")->setDeliverImmediately(true);

	frameCapacity = par("frameCapacity");

	qlenSignal = registerSignal("qlen");
	busySignal = registerSignal("busy");
	queueingTimeSignal = registerSignal("queueingTime");
	dropSignal = registerSignal("drop");
	txBytesSignal = registerSignal("txBytes");
//	rxBytesSignal = registerSignal("rxBytes");

	emit(qlenSignal, queue.getLength());
	emit(busySignal, false);
	isBusy = false;
}

void RandomQueue::startTransmitting(cMessage *msg)
{
	EV << "Starting transmission of " << msg << endl;
	isBusy = true;
	int64_t numBytes = check_and_cast<cPacket *>(msg)->getByteLength();
	send(msg, "link$o");

	emit(txBytesSignal, numBytes);


	// Schedule an event for the time when last bit will leave the gate.
	simtime_t endTransmission = gate("link$o")->getTransmissionChannel()->getTransmissionFinishTime();
	scheduleAt(endTransmission, endTransmissionEvent);
}

void RandomQueue::handleMessage(cMessage *msg)
{
//	std::cout << "Message queue : " << msg << endl;
	if (msg == endTransmissionEvent) {
			// Transmission finished, we can start next one.
			EV << "Transmission finished.\n";
			isBusy = false;
			if (queue.isEmpty()) {
					emit(busySignal, false);
			}
			else {
					msg = (cMessage *)queue.pop();
					emit(queueingTimeSignal, simTime() - msg->getTimestamp());
					emit(qlenSignal, queue.getLength());
					startTransmitting(msg);
					if(hasGUI())
						getParentModule()->bubble("Dequeue and sending packet...");

			}
	}
	else if (msg->arrivedOn("link$i")) {
			// pass up
//			emit(rxBytesSignal, (intval_t)check_and_cast<cPacket *>(msg)->getByteLength());
			send(msg, "out");
	}
	else {  // arrived on gate "in"
			if (endTransmissionEvent->isScheduled()) {
					// We are currently busy, so just queue up the packet.
					if (frameCapacity && queue.getLength() >= frameCapacity) {
							EV << "Received " << msg << " but transmitter busy and queue full: discarding\n";
							emit(dropSignal, (intval_t)check_and_cast<cPacket *>(msg)->getByteLength());
							delete msg;
							if(hasGUI())
								getParentModule()->bubble("Queue Full, drop packet...");
					}
					else if(!queue.isEmpty()){
							EV << "Received " << msg << " but transmitter busy: queuing up\n";
							msg->setTimestamp();
							int queueSize = queue.getLength();
							int positionInQueue = rand()%queueSize;
							cObject *objectBefMsg = queue.get(positionInQueue);
							queue.insertBefore(objectBefMsg, msg);
							emit(qlenSignal, queue.getLength());
//							std::cout << "Queuing packet " << msg << " in position : " << positionInQueue << endl;
							if(hasGUI())
								getParentModule()->bubble("Queuing packet...");
					}
					else{
						EV << "Received " << msg << " but transmitter busy: queuing up\n";
						msg->setTimestamp();
						queue.insert(msg);
						emit(qlenSignal, queue.getLength());
//						std::cout << "Queuing packet " << msg << endl;
						if(hasGUI())
							getParentModule()->bubble("Queuing packet...");
					}
			}
			else {
					// We are idle, so we can start transmitting right away.
					EV << "Received " << msg << endl;
					emit(queueingTimeSignal, SIMTIME_ZERO);
					startTransmitting(msg);
					emit(busySignal, true);
			}
	}
}

void RandomQueue::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, isBusy ? "transmitting" : "inactive");
    getDisplayString().setTagArg("i", 1, isBusy ? (queue.getLength() >= 5 ? "black" : "orange") : "");
}
