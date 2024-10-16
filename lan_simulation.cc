#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/netanim-module.h"  // Include for NetAnim
#include "ns3/mobility-module.h" // Include for Mobility




using namespace ns3;

//For colorful console printing
/*
 * Usage example :
 *    std::cout << BOLD_CODE << "some bold text << END_CODE << std::endl;
 *
 *    std::cout << YELLOW_CODE << BOLD_CODE << "some bold yellow text << END_CODE << std::endl;
 *
 */
#define YELLOW_CODE "\033[33m"
#define TEAL_CODE "\033[36m"
#define BOLD_CODE "\033[1m"
#define RED_CODE "\033[91m"
#define END_CODE "\033[0m"

uint32_t total_client_tx = 0;
uint32_t total_client_rx = 0;
uint32_t total_server_tx = 0;
uint32_t total_server_rx = 0;

void
CheckQueueSize (std::string context, uint32_t before, uint32_t after)
{
  std::cout << YELLOW_CODE << context << END_CODE << std::endl;
  std::cout << "\tTxQueue Size = " << after << std::endl;
}

void
BackoffTrace (std::string context, Ptr<const Packet> packet)
{
  std::cout << YELLOW_CODE << context << END_CODE << std::endl;
  EthernetHeader hdr;
  if (packet->PeekHeader (hdr))
    {
      std::cout << "\t" << Now() <<  " Packet from " << hdr.GetSource () << " to " << hdr.GetDestination () << " is experiencing backoff" << std::endl;
    }
}

void ClientTx (std::string context, Ptr<const Packet> packet)
{
  total_client_tx++;
}
void ClientRx (std::string context, Ptr<const Packet> packet)
{
  total_client_rx++;
}
void ServerTx (std::string context, Ptr<const Packet> packet)
{
  total_server_tx++;
}
void ServerRx (std::string context, Ptr<const Packet> packet)
{
  total_server_rx++;
}

int
main (int argc, char *argv[])
{
  CommandLine cmd;

  uint32_t n1 = 4;
  uint32_t n2 = 4;

  cmd.AddValue ("n1", "Number of LAN 1 nodes", n1);
  cmd.AddValue ("n2", "Number of LAN 2 nodes", n2);

  cmd.Parse (argc, argv);

  //LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
  //LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

  //For the first network
  NodeContainer lan1_nodes;

  //For the second network
  NodeContainer lan2_nodes;

  //for the nodes in the middle.
  NodeContainer router_nodes;

  lan1_nodes.Create (n1);
  lan2_nodes.Create (n2);
  router_nodes.Create (3);

  //Let's create LAN 1 by attaching a CsmaNetDevice to all the nodes on the LAN
  CsmaHelper csma1;
  csma1.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma1.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  //Router 1 is accessible on LAN 1, so we add it to the list nodes.
  lan1_nodes.Add (router_nodes.Get (0));
  //Actually attaching CsmaNetDevice to all LAN 1 nodes.
  NetDeviceContainer lan1Devices;
  lan1Devices = csma1.Install (lan1_nodes);

  //Doing the same for LAN 2
  CsmaHelper csma2;
  csma2.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma2.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));
  //Router 2 is on LAN 2, so we add it to the node container
  lan2_nodes.Add (router_nodes.Get (1));

  NetDeviceContainer lan2Devices;
  lan2Devices = csma2.Install (lan2_nodes);

  /* So far our two LANs are disjoint, r1 and r2 need to be connected */
  //A PointToPoint connection between the two routers
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer firstHopLinkDevs;
  firstHopLinkDevs = pointToPoint.Install (router_nodes.Get(0), router_nodes.Get(2));

  //routerDevices = 
  NetDeviceContainer secondHopLinkDevs;
  secondHopLinkDevs = pointToPoint.Install (router_nodes.Get(2), router_nodes.Get(1));


  //Setting IP addresses. Notice that router 1 & 2 are in LAN 1 & 2 respectively.
  InternetStackHelper stack;
  stack.Install (lan1_nodes);
  stack.Install (lan2_nodes);
  stack.Install (router_nodes.Get(2));
  
  Ipv4AddressHelper address;
  //For LAN 1
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer lan1interfaces;
  lan1interfaces = address.Assign (lan1Devices);
  //For LAN 2
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer lan2interfaces;
  lan2interfaces = address.Assign (lan2Devices);
  //For PointToPoint
  address.SetBase ("10.1.100.0", "255.255.255.0");
  Ipv4InterfaceContainer routerInterfaces;
  routerInterfaces = address.Assign (firstHopLinkDevs);

    address.SetBase ("10.1.200.0", "255.255.255.0");
  Ipv4InterfaceContainer routerInterfaces2;
  routerInterfaces2 = address.Assign (secondHopLinkDevs);

  //Let's install a UdpEchoServer on all nodes of LAN2
  UdpEchoServerHelper echoServer (9);
  ApplicationContainer serverApps = echoServer.Install (lan2_nodes);
  serverApps.Start (Seconds (0));
  serverApps.Stop (Seconds (20));

  //Let's create UdpEchoClients in all LAN1 nodes.
  UdpEchoClientHelper echoClient (lan2interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (MilliSeconds (200)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //We'll install UdpEchoClient on two nodes in lan1 nodes
  NodeContainer clientNodes (lan1_nodes.Get(0), lan1_nodes.Get(1));

  ApplicationContainer clientApps = echoClient.Install (clientNodes);
  clientApps.Start (Seconds (0));
  clientApps.Stop (Seconds (20));

  //For routers to be able to forward packets, they need to have routing rules.
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  csma1.EnablePcap("lan1", lan1Devices);
  csma2.EnablePcap("lan2", lan2Devices);
  pointToPoint.EnablePcapAll("routers");
  pointToPoint.EnableAscii("ascii-p2p", router_nodes);

  Config::Connect("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/TxQueue/PacketsInQueue", MakeCallback(&CheckQueueSize));
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/TxQueue/PacketsInQueue", MakeCallback(&CheckQueueSize));
  Config::Connect("/NodeList/*/DeviceList/*/$ns3::CsmaNetDevice/MacTxBackoff", MakeCallback(&BackoffTrace));
  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/Tx", MakeCallback(&ClientTx));
  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/Rx", MakeCallback(&ClientRx));
  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpEchoServer/Tx", MakeCallback(&ServerTx));
  Config::Connect("/NodeList/*/ApplicationList/*/$ns3::UdpEchoServer/Rx", MakeCallback(&ServerRx));
  
  MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    
    // Assign positions for LAN 1 nodes 
    positionAlloc->Add(Vector(12.0, 12.0, 0.0)); // Node 0
    positionAlloc->Add(Vector(12.0, 37.0, 0.0)); // Node 1
    positionAlloc->Add(Vector(12.0, 62.0, 0.0)); // Node 2
    positionAlloc->Add(Vector(12.0, 0.0, 0.0)); // Node 3
    
    // Assign positions for routers
    positionAlloc->Add(Vector(37.0, 50.0, 0.0)); // Router 1
    positionAlloc->Add(Vector(50.0, 50.0, 0.0)); // Router 2
    positionAlloc->Add(Vector(62.0, 50.0, 0.0)); // Router 3

    // Assign positions for LAN 2 nodes
    positionAlloc->Add(Vector(87.0, 12.0, 0.0)); // Node 7
    positionAlloc->Add(Vector(87.0, 37.0, 0.0)); // Node 8
    positionAlloc->Add(Vector(87.0, 50.0, 0.0)); // Node 9
    positionAlloc->Add(Vector(87.0, 62.0, 0.0)); // Node 10

    mobility.SetPositionAllocator(positionAlloc);
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(lan1_nodes);
    mobility.Install(lan2_nodes);
    mobility.Install(router_nodes);



  Simulator::Stop (Seconds (20));

  // Add this section before Simulator::Run()
  AnimationInterface anim("my_simulation.xml");  // Creates an animation file
  
  for (uint32_t i = 0; i < lan1_nodes.GetN(); i++) {
     anim.SetConstantPosition(lan1_nodes.Get(i), i * 10.0, 10.0);  // x position increases with node ID, fixed y position
   }

   for (uint32_t i = 0; i < lan2_nodes.GetN(); i++) {
     anim.SetConstantPosition(lan2_nodes.Get(i), (i + lan1_nodes.GetN()) * 10.0, 20.0);  // same idea for LAN 2 nodes
   }

   for (uint32_t i = 0; i < router_nodes.GetN(); i++) {
     anim.SetConstantPosition(router_nodes.Get(i), (i + lan1_nodes.GetN() + lan2_nodes.GetN()) * 10.0, 15.0);  // for router nodes
   }

  // Assign colors to all nodes in the system.
  // Color format: UpdateNodeColor(nodeId, R, G, B)

  // For LAN 1 nodes
  anim.UpdateNodeColor(0, 255, 0, 0);   // Node 0 in red
  anim.UpdateNodeColor(1, 255, 0, 0);   // Node 1 in red
  anim.UpdateNodeColor(2, 255, 0, 0);   // Node 2 in red
  anim.UpdateNodeColor(3, 255, 0, 0); // Node 3 in red

  // For router nodes
  anim.UpdateNodeColor(4, 0, 255, 0); // Router 1 in green
  anim.UpdateNodeColor(5, 0, 255, 0); // Router 2 in green
  anim.UpdateNodeColor(6, 0, 255, 0); // Router 3 in green

  // For LAN 2 nodes
  anim.UpdateNodeColor(7, 0, 0, 255); // Node 7 in blue
  anim.UpdateNodeColor(8, 0, 0, 255); // Node 8 in blue
  anim.UpdateNodeColor(9, 0, 0, 255);   // Node 9 in blue
  anim.UpdateNodeColor(10, 0, 0, 255);  // Node 10 in blue
  



  Simulator::Run ();

  std::cout << "Client Tx: " << total_client_tx << "\tClient Rx: " << total_client_rx << std::endl;
  std::cout << "Server Rx: " << total_server_rx << std::endl;

  Simulator::Destroy ();
  return 0;

}