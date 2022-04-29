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

#include "Hosting.h"

Define_Module(Hosting);

Hosting::~Hosting()
{
	// TODO Auto-generated destructor stub
	cancelAndDelete(endTransmissionEvent);
}

void Hosting::initialize()
{
	// get the pointer of the parameter and signals of own module
	myAddress = getParentModule()->par("address");
	endTransmissionEvent = new cMessage("endTransmissionEvent");

	sourceAddress = registerSignal("sourceAddress");
	dropSignal = registerSignal("drop");

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
	nedTypes.push_back("src.device.Switch");
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

			destAddressesList.push_back(topo->getNode(i)->getModule()->par("address"));
	}
	delete topo;
}

void Hosting::handleMessage(cMessage *msg)
{
  std::cout << "Hosting recoit msg : " << msg << endl;

  if(msg == endTransmissionEvent){
  	EV << "Transmission finished.\n";
  	isBusy = false;
  	return;
  }
  else if(msg->arrivedOn("localIn")){
		isBusy = false;
		startTransmitting(msg);
		return;
  }

  Packet *pk = check_and_cast<Packet *>(msg);
  int destAddr = pk->getDestAddr();
  if(destAddr == myAddress && msg->arrivedOn("interface$i")){
  	EV << "local delivery of packet " << pk->getName() << endl;
		send(pk, "localOut");
  }
}

/**
 * Begin of the message transmission through the interfaces gate.
 *
 * @param *msg The message to transmit through the host.
 */
void Hosting::startTransmitting(cMessage *msg)
{
    EV << "Starting transmission of " << msg << endl;
    std::cout << "Starting transmission of " << msg << endl;
    isBusy = true;

    Packet *pk = check_and_cast<Packet *>(msg);
		int destAddr = pk->getDestAddr();
//    int destAddr = 13;

		if (*std::find(destAddressesList.begin(), destAddressesList.end(), destAddr) == destAddr) {
				EV << "forwarding packet " << pk->getName() << endl;
				send(pk, "interface$o");
				// Schedule an event for the time when last bit will leave the gate.
				simtime_t endTransmission = gate("interface$o")->getTransmissionChannel()->getTransmissionFinishTime();
				scheduleAt(endTransmission, endTransmissionEvent);

				if (hasGUI())
					getParentModule()->bubble("Sending packet...");

				return;
			}

		EV << "address " << destAddr << " unreachable, discarding packet " << pk->getName() << endl;
		emit(dropSignal, (intval_t)pk->getByteLength());
		delete pk;

}

/**
 * Refresh display one the graphical simulation
 */
void Hosting::refreshDisplay() const
{
    getDisplayString().setTagArg("t", 0, isBusy ? "transmitting" : "inactive");
}

