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

#include "Switching.h"

Define_Module(Switching);

Switching::~Switching(){
}

void Switching::initialize()
{
	// get the pointer of the parameter and signals of own module
	myAddress = getParentModule()->par("address");
	endTransmissionEvent = new cMessage("endTransmissionEvent");

	sourceAddress = registerSignal("sourceAddress");
	dropSignal = registerSignal("drop");
	outputIfSignal = registerSignal("outputIf");

	//
	// Brute force approach -- every node does topology discovery on its own,
	// and finds routes to all other nodes independently, at the beginning
	// of the simulation. This could be improved: (1) central routing database,
	// (2) on-demand route calculation
	//
	cTopology *topo = new cTopology("topo");

	std::vector<std::string> nedTypes;
	nedTypes.push_back(getParentModule()->getNedTypeName());
	nedTypes.push_back("src.device.Router");
	topo->extractByNedTypeName(nedTypes);
	EV << "cTopology found " << topo->getNumNodes() << " nodes\n";

	cTopology::Node *thisNode = topo->getNodeFor(getParentModule());

	// find and store next hops
	for (int i = 0; i < topo->getNumNodes(); i++) {
			if (topo->getNode(i) == thisNode){
					continue;  // skip ourselves
			}
			topo->calculateUnweightedSingleShortestPathsTo(topo->getNode(i));

			if (thisNode->getNumPaths() == 0){
				  EV << "  this not connected with others nodes" << endl;
					continue;  // not connected
			}

			cGate *parentModuleGate = thisNode->getPath(0)->getLocalGate();
			int gateIndex = parentModuleGate->getIndex();
			int address = topo->getNode(i)->getModule()->par("address");
			switchingTable[address] = gateIndex;
			EV << "  towards address " << address << " gateIndex is " << gateIndex << endl;
	}
	delete topo;
}

void Switching::handleMessage(cMessage *msg)
{
	if (msg == endTransmissionEvent) {
		// Transmission finished, we can start next one.
		EV << "Transmission finished.\n";
		isBusy = false;
		startTransmitting(msg);
	}
	// if a message arrived from out Switch, we send it to the App
	else if(msg->arrivedOn("interfaces$i", 0)){
		send(msg, "localOut");
	}
	else{
		if (endTransmissionEvent->isScheduled()) {
			msg->setTimestamp();
		}
		else{
			startTransmitting(msg);
		}
	}
}

/**
 * Begin of the message transmission through the interfaces gate.
 *
 * @param *msg The message to transmit through the fifo.
 */
void Switching::startTransmitting(cMessage *msg)
{
    EV << "Starting transmission of " << msg << endl;
    isBusy = true;

    if(check_and_cast<cMessage *>(msg))	return;

    Packet *pk = check_and_cast<Packet *>(msg);
		int destAddr = pk->getDestAddr();

		if (destAddr == myAddress) {
				EV << "local delivery of packet " << pk->getName() << endl;
				send(pk, "localOut");
				emit(outputIfSignal, -1);  // -1: local
				return;
		}

		SwitchingTable::iterator it = switchingTable.find(destAddr);
		if (it == switchingTable.end()) {
				EV << "address " << destAddr << " unreachable, discarding packet " << pk->getName() << endl;
				emit(dropSignal, (intval_t)pk->getByteLength());
				delete pk;
				return;
		}

		int outGateIndex = (*it).second;
		EV << "forwarding packet " << pk->getName() << " on gate index " << outGateIndex << endl;
		pk->setHopCount(pk->getHopCount()+1);
		emit(outputIfSignal, outGateIndex);
		isBusy = true;
		// gives the exactly gate with '$o' to indicate the port in output mode.
		send(pk, "interfaces$o", outGateIndex);

    // Schedule an event for the time when last bit will leave the gate.
    simtime_t endTransmission = gate("interfaces$o", 0)->getTransmissionChannel()->getTransmissionFinishTime();
    scheduleAt(endTransmission, endTransmissionEvent);
}

