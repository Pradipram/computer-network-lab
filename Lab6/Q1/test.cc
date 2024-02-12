/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    // CommandLine cmd(__FILE__);
    // cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //creating nodes individual as well as combined
    int numNodes = 5;//default values
    CommandLine cmd;
    cmd.AddValue ("numNodes", "Number of nodes in the ring topology", numNodes);
    cmd.Parse (argc, argv);

    // Check if the number of nodes is within the valid range
    if (numNodes < 3 || numNodes > 8) {
        NS_LOG_ERROR ("Number of nodes must be between 3 and 8");
        return 1;
    }

    // NodeContainer nodes,node01,node12,node23,node30;
    NodeContainer nodes;
    NodeContainer indNodes[numNodes];
    nodes.Create(numNodes);

    // node01.Add(nodes.Get(0));
    // node01.Add(nodes.Get(1));

    // node12.Add(nodes.Get(1));
    // node12.Add(nodes.Get(2));

    // node23.Add(nodes.Get(2));
    // node23.Add(nodes.Get(3));

    // node30.Add(nodes.Get(3));
    // node30.Add(nodes.Get(0));

    for(int i=0;i<numNodes;i++)
    {
        indNodes[i].Add(nodes.Get(i));
        indNodes[i].Add(nodes.Get((i+1)%numNodes));
    }

    //making links
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));


    //making devices
    // NetDeviceContainer device01,device12,device23,device30;
    // device01 = pointToPoint.Install(node01);
    // device12 = pointToPoint.Install(node12);
    // device23 = pointToPoint.Install(node23);
    // device30 = pointToPoint.Install(node30);
    NetDeviceContainer devices[numNodes];
    for(int i=0;i<numNodes;i++)
    {
        devices[i] = pointToPoint.Install(indNodes[i]);
    }
  
  
    //installing ip stack on all the nodes.
    InternetStackHelper stack;
    stack.Install(nodes);

    //assignment of ip address
    Ipv4AddressHelper address;
    Ipv4InterfaceContainer interfaces[numNodes];
    // address.SetBase("10.1.1.0", "255.255.255.0");
    // Ipv4InterfaceContainer interfaces01 = address.Assign(device01);

    // address.SetBase("11.1.1.0", "255.255.255.0");
    // Ipv4InterfaceContainer interfaces12 = address.Assign(device12);
    //     address.SetBase("12.1.1.0", "255.255.255.0");
    // Ipv4InterfaceContainer interfaces23 = address.Assign(device23);

    // address.SetBase("13.1.1.0", "255.255.255.0");
    // Ipv4InterfaceContainer interfaces30 = address.Assign(device30);
    for(int i=0;i<numNodes;i++)
    {
        std::string baseAddress = "10." + std::to_string(i+1) + ".1.0";
        address.SetBase(baseAddress.c_str(), "255.255.255.0");
        interfaces[i] = address.Assign(devices[i]);
    }

    //configuration of server
    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));


    //configuration of clients
    UdpEchoClientHelper echoClient(interfaces[0].GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(4));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(3));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));


    //Creating xml file for simulation
    AnimationInterface anim("ringTopology.xml");
    pointToPoint.EnablePcapAll("ring");;

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
