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


//
// A ten-node network topology
//
network Network5
{
    types:
        channel C extends DatarateChannel
        {
            delay = uniform(0.001ms, 1ms);
            datarate = 1000000kbps;
        }
    submodules:
        routers[5]: Router {
            address = index;
        }
        switchs[5]: Switch {
            address = index;
        }

    connections:
        routers[1].port++ <--> C <--> routers[0].port++;
        routers[0].port++ <--> C <--> routers[2].port++;
        routers[4].port++ <--> C <--> routers[3].port++;
        routers[3].port++ <--> C <--> routers[1].port++;
        routers[3].port++ <--> C <--> routers[2].port++;
        routers[2].port++ <--> C <--> routers[4].port++;
        routers[1].port++ <--> C <--> routers[2].port++;

        switchs[0].ports++ <--> C <--> routers[0].port++;
        switchs[1].ports++ <--> C <--> routers[1].port++;
        switchs[2].ports++ <--> C <--> routers[2].port++;
        switchs[3].ports++ <--> C <--> routers[3].port++;
        switchs[4].ports++ <--> C <--> routers[2].port++;

}
