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

#ifndef BEHAVIORDEVICE_HOSTING_H_
#define BEHAVIORDEVICE_HOSTING_H_

#include <iostream>
#include <map>
#include <algorithm>
#include <omnetpp.h>

#include "C:\Users\zacha\omnetpp-6.0-windows-x86_64\samples\TER-gestion-files-d-attente\src\application\Packet_m.h"

using namespace omnetpp;

class Hosting : public cSimpleModule
{
	private:
		int myAddress;

		std::vector<int> destAddressesList;

		cMessage *endTransmissionEvent = nullptr;
		bool isBusy;

		simsignal_t sourceAddress;
		simsignal_t dropSignal;

	public:
		virtual ~Hosting();
	protected:
		virtual void initialize() override;
		virtual void handleMessage(cMessage *msg) override;
		virtual void startTransmitting(cMessage *msg);
		virtual void refreshDisplay() const override;
};

#endif /* BEHAVIORDEVICE_HOSTING_H_ */
