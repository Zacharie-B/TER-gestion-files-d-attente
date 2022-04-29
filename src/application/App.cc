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

#include "App.h"

Define_Module(App);

App::~App()
{
    cancelAndDelete(generatePacket);
}

void App::initialize()
{
    myAddress = par("address");
    packetLengthBytes = &par("packetLength");
    sendIATime = &par("sendIaTime");  // volatile parameter
    pkCounter = 0;

    WATCH(pkCounter);
    WATCH(myAddress);

    const char *destAddressesPar = par("destAddresses");
    cStringTokenizer tokenizer(destAddressesPar);
    const char *token;
    while ((token = tokenizer.nextToken()) != nullptr)
        destAddresses.push_back(atoi(token));

    const char *srcAddressesPar = par("srcAddresses");
    cStringTokenizer tokenizerr(srcAddressesPar);
    const char *tokenSA;
    while((tokenSA = tokenizerr.nextToken()) != nullptr)
    		srcAddresses.push_back(atoi(tokenSA));

    if (destAddresses.size() == 0)
        throw cRuntimeError("At least one address must be specified in the destAddresses parameter!");

    generatePacket = new cMessage("nextPacket");
    scheduleAt(sendIATime->doubleValue(), generatePacket);

    endToEndDelaySignal = registerSignal("endToEndDelay");
    hopCountSignal = registerSignal("hopCount");
    sourceAddressSignal = registerSignal("sourceAddress");
}

void App::handleMessage(omnetpp::cMessage *msg)
{
    if (msg == generatePacket && *std::find(srcAddresses.begin(), srcAddresses.end(), myAddress) == myAddress) {
        // Sending packet
        int destAddress = destAddresses[intuniform(0, destAddresses.size()-1)];

        char pkname[40];
        sprintf(pkname, "pk-%d-to-%d-#%ld", myAddress, destAddress, pkCounter++);
        EV << "generating packet " << pkname << endl;

        Packet *pk = new Packet(pkname);
        pk->setByteLength(packetLengthBytes->intValue());
        pk->setKind(intuniform(0, 7));
        pk->setSrcAddr(myAddress);
        pk->setDestAddr(destAddress);
        send(pk, "out");

      	std::cout << "App envoie msg : " << msg << endl;

        scheduleAt(simTime() + sendIATime->doubleValue(), generatePacket);
        if (hasGUI())
            getParentModule()->bubble("Generating packet...");
    }
    else if(msg == generatePacket){
    	// Dodge the issue which read packet whereas there is nothing to read.
    	std::cout << "App ne fait rien" << endl;
    }
    else {
        // Handle incoming packet
        Packet *pk = check_and_cast<Packet *>(msg);
        EV << "received packet " << pk->getName() << " after " << pk->getHopCount() << "hops" << endl;
        emit(endToEndDelaySignal, simTime() - pk->getCreationTime());
        emit(hopCountSignal, pk->getHopCount());
        emit(sourceAddressSignal, pk->getSrcAddr());
        delete pk;

        if (hasGUI())
            getParentModule()->bubble("Arrived!");
    }
}

