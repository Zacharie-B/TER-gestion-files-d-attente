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

package ter_gestion_files_d_attente.QueueComponents;

import inet.queueing.contract.IPacketQueue;

module DropTailQueuePpp
{
	parameters:
	    string interfaceTableModule;   // The path to the InterfaceTable module
        string displayStringTextFormat = default("rate: %b\nsent: %s, rcvd: %r\nqueue: %q, drop: %d");
        bool sendRawBytes = default(false); // when true packets are serialized into a sequence of bytes before sending out
        int mtu @unit(B) = default(4470B);
        @lifecycleSupport;
        double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
        double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
        @class(QueuePpp);
        @display("i=block/rxtx");

        @signal[transmissionStateChanged](type=long);    // 1:transmit, 0:idle
        @signal[rxPkOk](type=inet::Packet);
        @signal[packetDropped](type=inet::Packet);
        @signal[packetSentToLower](type=inet::Packet);
        @signal[packetReceivedFromLower](type=cPacket);
        @signal[packetSentToUpper](type=cPacket);
        @signal[packetReceivedFromUpper](type=cPacket);
        @statistic[transmissionState](title="tx state"; source=transmissionStateChanged; record=timeavg,vector; interpolationmode=sample-hold);
        @statistic[txPk](title="packets transmitted"; source=packetSentToLower; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rxPkOk](title="packets received OK"; source=rxPkOk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[packetDropIncorrectlyReceived](title="packet drop: incorrectly received"; source=packetDropReasonIsIncorrectlyReceived(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[packetDropInterfaceDown](title="packet drop: interface down"; source=packetDropReasonIsInterfaceDown(packetDropped); record=count,sum(packetBytes),vector(packetBytes); interpolationmode=none);
        @statistic[rcvdPkFromHl](title="packets received from higher layer"; source=packetReceivedFromUpper; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[passedUpPk](title="packets passed to higher layer"; source=packetSentToUpper; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input inGate;
        output outGate;
        
    submodules:
        dropTailQueue: <default("DropTailQueue")> like IPacketQueue {
            parameters:
                packetCapacity = default(10000);
                @display("p=90,70;q=lqueue");
        }
}    