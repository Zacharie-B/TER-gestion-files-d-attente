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

#ifndef DEVICE_SWITCH_H_
#define DEVICE_SWITCH_H_

#include <map>
#include <omnetpp.h>

#include "C:\Users\zacha\omnetpp-6.0-windows-x86_64\samples\TER-gestion-files-d-attente\src\application\Packet_m.h"

using namespace omnetpp;

class Switching : public cSimpleModule
{
	private:
		int myAddress;

		typedef std::map<int, int> SwitchingTable;  // destaddr -> gateindex
		SwitchingTable switchingTable;

		simsignal_t sourceAddress;
		simsignal_t dropSignal;
		simsignal_t outputIfSignal;

	protected:
		virtual void initialize() override;
		virtual void handleMessage(cMessage *msg) override;
};

#endif /* DEVICE_SWITCH_H_ */
