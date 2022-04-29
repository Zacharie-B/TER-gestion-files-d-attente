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
import src.behaviorDevice.Hosting;

//
// TODO documentation
//
module Host
{
    parameters:
        int address;
        string appType;
        @display("i=device/pc2");
    gates:
        inout ports[];
    submodules:
        app: <appType> like IApp {
            parameters:
                address = parent.address;
                @display("p=140,40");
        }
        hosting: Hosting {
            parameters:
                @display("p=140,130");
            gates:
                interfaces[sizeof(parent.ports)];
        }
    connections:
        hosting.localOut --> app.in;
        hosting.localIn <-- app.out;
		for i=0..sizeof(ports)-1{
            ports[i] <--> hosting.interfaces[i];
        }
        
}