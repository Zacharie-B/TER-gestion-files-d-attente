//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
package simulations;

import src.device.Router;
import ned.DatarateChannel;
import src.device.Switch;
import src.device.Host;


//
// A ten-node network topology
//
network Network5
{
    parameters:
        @display("bgb=900,520");
    types:
        channel C extends DatarateChannel
        {
            delay = uniform(0.0001s, 0.01s);
            datarate = 1Gbps;
        }
    submodules:
        routers[5]: Router {
            address = index;
        }
        switchs[5]: Switch {
            address = index + 5;
        }
        hosts[20]: Host {
            address = index + 10;
        }

    connections:
        routers[1].port++ <--> C <--> routers[0].port++;
        routers[3].port++ <--> C <--> routers[1].port++;
        routers[1].port++ <--> C <--> routers[4].port++;
        routers[3].port++ <--> C <--> routers[0].port++;
        routers[3].port++ <--> C <--> routers[2].port++;
        routers[4].port++ <--> C <--> routers[2].port++;

        switchs[0].ports++ <--> C <--> routers[0].port++;
        switchs[1].ports++ <--> C <--> routers[1].port++;
        switchs[2].ports++ <--> C <--> routers[2].port++;
        switchs[3].ports++ <--> C <--> routers[3].port++;
        switchs[4].ports++ <--> C <--> routers[2].port++;
        
        hosts[0].port <--> C <--> switchs[0].ports++;
        hosts[1].port <--> C <--> switchs[1].ports++;
        hosts[2].port <--> C <--> switchs[1].ports++;
        hosts[3].port <--> C <--> switchs[1].ports++;
        hosts[4].port <--> C <--> switchs[1].ports++;
        hosts[5].port <--> C <--> switchs[1].ports++;
        hosts[6].port <--> C <--> switchs[1].ports++;
        hosts[7].port <--> C <--> switchs[1].ports++;
        hosts[8].port <--> C <--> switchs[1].ports++;
        hosts[9].port <--> C <--> switchs[1].ports++;
        hosts[10].port <--> C <--> switchs[1].ports++;
        hosts[11].port <--> C <--> switchs[2].ports++;
        hosts[12].port <--> C <--> switchs[2].ports++;
        hosts[13].port <--> C <--> switchs[2].ports++;
        hosts[14].port <--> C <--> switchs[2].ports++;
        hosts[15].port <--> C <--> switchs[2].ports++;
        hosts[16].port <--> C <--> switchs[2].ports++;
        hosts[17].port <--> C <--> switchs[3].ports++;
        hosts[18].port <--> C <--> switchs[4].ports++;
        hosts[19].port <--> C <--> switchs[4].ports++;
        
}
