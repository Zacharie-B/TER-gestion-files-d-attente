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
//
// TODO documentation
//
module Switch
{
    parameters:
        int address;
        string appType;
        int numberPorts = default(1);

        @display("i=device/switch");
    gates:
        inout ports[numberPorts];
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
                interfaces[sizeof(parent.ports)];
        }
    connections:
        switching.localOut --> app.in;
        switching.localIn <-- app.out;
        ports[0] <--> switching.interfaces[0];
}

