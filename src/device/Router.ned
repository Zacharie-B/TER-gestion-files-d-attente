//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//


package src.device;

import src.behaviorDevice.Routing;
import src.queue.IQueue;
import src.application.IApp;
import src.application.App;

//
// A "Node" consists of a Routing plus an App module which represents the
// aggregate traffic of the subnets connected to that router.
//
module Router
{
    parameters:
        int address;
        string appType;
        string queueType;
        @display("i=old/srouter,gold");
    gates:
        inout port[];
    submodules:
        app: <appType> like IApp {
            parameters:
                address = parent.address;
                @display("p=140,60");
        }
        routing: Routing {
            parameters:
                @display("p=140,130");
            gates:
                in[sizeof(parent.port)];
                out[sizeof(parent.port)];
        }
        queue[sizeof(port)]: <queueType> like IQueue {
            parameters:
                @display("p=80,200,row");
        }
    connections:
        routing.localOut --> app.in;
        routing.localIn <-- app.out;
        for i=0..sizeof(port)-1 {
            routing.out[i] --> queue[i].in;
            routing.in[i] <-- queue[i].out;
            queue[i].link <--> port[i];
        }
}


