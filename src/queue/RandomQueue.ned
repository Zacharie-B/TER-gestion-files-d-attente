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

package src.queue;

//
// TODO documentation
//
simple RandomQueue like IQueue
{
    parameters:
//        int frameLength = default(1500); // max length of a frame
        int frameCapacity = default(3); // max number of packets; 0 means no limit
        bool useCutThroughSwitching = default(false);  // use cut-through switching instead of store-and-forward
        @display("i=block/queue;q=queue");
    	@signal[qlen](type="long");
        @signal[busy](type="bool");
        @signal[queueingTime](type="simtime_t");
        @signal[drop](type="long");
        @signal[txBytes](type="long");
//        @signal[rxBytes](type="long");
        @statistic[qlen](title="queue length";record=vector?,timeavg,max;interpolationmode=sample-hold);
        @statistic[busy](title="server busy state";record=vector?,timeavg;interpolationmode=sample-hold);
        @statistic[queueingTime](title="queueing time at dequeue";unit=s;record=vector?,timeavg,max;interpolationmode=sample-hold);
        @statistic[drop](title="dropped packet byte length";unit=bytes;record=vector?,count,sum;interpolationmode=none);
        @statistic[txBytes](title="transmitting packet byte length";unit=bytes;record=vector?,count,sum,histogram;interpolationmode=none);
//        @statistic[rxBytes](title="received packet byte length";unit=bytes;record=vector?,count,sum,histogram;interpolationmode=none);
    gates:
        input in;
        output out;
        inout link;
}