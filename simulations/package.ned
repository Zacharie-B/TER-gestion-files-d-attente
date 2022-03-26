package gestionfilesattentes;

import inet.networklayer.configurator.ipv4.Ipv4FlatNetworkConfigurator;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import inet.queueing.queue.DropTailQueue;
import inet.queueing.sink.ActivePacketSink;
import inet.queueing.source.ActivePacketSource;


@license(LGPL);
//
// TODO documentation
//
network Network
{
    @display("bgb=440,264");
    submodules:
        activePacketSource: ActivePacketSource {
            @display("p=68,126");
        }
        dropTailQueue: DropTailQueue {
            @display("p=219,126");
        }
        passivePacketSink: ActivePacketSink {
            @display("p=357,126");
        }
    connections:
        activePacketSource.out --> dropTailQueue.in;
        dropTailQueue.out --> passivePacketSink.in;
}

//
// TODO documentation
//
network testQueueRouter
{
    @display("bgb=440,200");
    submodules:
        router: Router {
            @display("p=214,80");
        }
        standardHost: StandardHost {
            @display("p=67,80");
        }
        standardHost1: StandardHost {
            @display("p=349,80");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=214,147");
        }
    connections:

        standardHost.pppg[0] <--> Eth100M <--> router.pppg[0];
        standardHost1.pppg[0] <--> Eth100M <--> router.pppg[1];
}

