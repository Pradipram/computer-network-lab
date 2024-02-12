#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/random-variable-stream.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("MultipleClientsExample");

int main (int argc, char *argv[])
{
  // Command line parameters
  uint32_t nClients = 1;  // Default number of clients
  uint32_t nPackets = 5;  // Default number of packets per client

  CommandLine cmd;
  cmd.AddValue ("nClients", "Number of clients", nClients);
  cmd.AddValue ("nPackets", "Number of packets per client", nPackets);
  cmd.Parse (argc, argv);

  // Set simulation time
  Time::SetResolution (Time::NS);
  double simTime = 20.0; // seconds

  // Create nodes (1 server + nClients clients)
  NodeContainer nodes;
  nodes.Create (nClients + 1);

  // Create point-to-point links between server and clients
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("5ms"));

  NetDeviceContainer serverDevices;
  serverDevices = pointToPoint.Install (nodes.Get (0), nodes.Get (1));

  NetDeviceContainer clientDevices;
  for (uint32_t i = 1; i <= nClients; ++i) {
    NetDeviceContainer clientDevice = pointToPoint.Install (nodes.Get (i), nodes.Get (0));
    clientDevices.Add (clientDevice);
  }

  // Install Internet stack
  InternetStackHelper stack;
  stack.Install (nodes);

  // Assign IP addresses
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  address.Assign (serverDevices);
  address.SetBase ("10.2.1.0", "255.255.255.0");
  address.Assign (clientDevices);

  // Create applications for each client
  uint16_t port = 9;

  for (uint32_t i = 1; i <= nClients; ++i) {
    OnOffHelper onoff ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.1.1.1"), port));
    onoff.SetAttribute ("PacketSize", UintegerValue (1024));
    onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));

    ApplicationContainer apps = onoff.Install (nodes.Get (i));
    apps.Start (Seconds (rand()%6+2));
    apps.Stop (Seconds (simTime + 1.0));
  }

  // Create animation object
  AnimationInterface anim ("multiple-clients-animation.xml");

  // Set up tracing
  pointToPoint.EnablePcapAll ("multiple-clients");

  // Run simulation
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

