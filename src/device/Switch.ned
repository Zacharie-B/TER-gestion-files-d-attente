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

package src.device;

import src.application.App;
import src.application.IApp;
import src.behaviorDevice.Switching;
import src.queue.FifoQueue;

//
// TODO documentation
//
module Switch
{
    parameters:
        int address;
        string appType;
        @display("i=device/switch");
    gates:
        inout ports[];
    submodules:
        app: <appType> like IApp {
            parameters:
                address = parent.address;
                @display("p=140,40");
        }
        switching: Switching {
            parameters:
                @display("p=140,130");
            gates:
                in[sizeof(parent.ports)];
                out[sizeof(parent.ports)];
        }
        queue[sizeof(ports)]: FifoQueue {
            parameters:
                @display("p=80,200,row");
        }
    connections:
        switching.localOut --> app.in;
        switching.localIn <-- app.out;
        for i=0..sizeof(ports)-1{
            switching.out[i] --> queue[i].in;
            switching.in[i] <-- queue[i].out;
            queue[i].link <--> ports[i];
        }
}

