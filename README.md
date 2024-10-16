<<<<<<< HEAD
Network Simulation using NS-3 with NetAnim Visualization

This project simulates a simple network using the NS-3 simulator. The simulation involves two Local Area Networks (LANs), each connected to a central router. The topology and node arrangement are visualized using NetAnim.
Topology Overview

The network consists of:

    LAN 1: Contains multiple nodes connected through a CSMA (Carrier Sense Multiple Access) protocol. All nodes are vertically aligned with the same x-coordinate, creating a vertical LAN structure.
    LAN 2: Similar to LAN 1, but positioned further along the x-axis, also in a vertical alignment.
    Routers: Three routers are positioned between the two LANs, connected to both LAN 1 and LAN 2 using point-to-point (P2P) links. The routers are aligned horizontally in the middle of the two LANs, facilitating communication between them.

The simulation generates traffic between nodes on LAN 1 and LAN 2 using a UDP Echo Client-Server model. This demonstrates basic communication and routing within the network.
Key Components:

    LAN 1 (n1): A set of nodes connected using a CSMA channel with 100 Mbps bandwidth and a small delay. These nodes are positioned vertically along the y-axis.
    LAN 2 (n2): Another set of nodes connected in a similar fashion to LAN 1, but placed further along the x-axis, with all nodes also aligned vertically.
    Routers: Three routers facilitate communication between LAN 1 and LAN 2 using point-to-point links.

Node Positioning

In the simulation:

    LAN 1 Nodes: Positioned along the same x-axis (x = 10.0) with a vertical distance of 10 units between each node.
    LAN 2 Nodes: Positioned similarly, but with an x-coordinate of 50.0, creating a clear visual separation from LAN 1.
    Routers: Positioned between LAN 1 and LAN 2 at an x-coordinate of 30.0, with their y-coordinates spaced in alignment with the LAN nodes.

The positioning of nodes is managed through the SetConstantPosition function, ensuring a clear and structured network layout in the NetAnim visualization tool.
Project Features

    NS-3 Simulation: The simulation uses NS-3 to create and run the network. The simulation includes traffic generation and the use of Internet protocols to handle routing.
    NetAnim Visualization: Node positions are visualized using NetAnim, an NS-3 tool that helps users see the topology and the flow of packets between nodes.
    UDP Communication: The simulation implements a UDP Echo Server on LAN 2 and an Echo Client on LAN 1, simulating network traffic.

Running the Simulation

To run this simulation, follow these steps:

    Clone the repository:

    bash

git clone https://github.com/yourusername/yourproject.git

Build the simulation using NS-3:

bash

./waf configure
./waf

Run the simulation:

bash

    ./waf --run "your-script-name"

    Visualize the results in NetAnim: After running the simulation, an XML file (network_animation.xml) will be generated. You can open this file using NetAnim to view the network topology and observe the communication between nodes.

Customizing the Simulation

The number of nodes in each LAN can be modified by adjusting the n1 and n2 parameters in the script:

cpp

=======
# **Network Simulation using NS-3 with NetAnim Visualization**

This project simulates a simple network using the NS-3 simulator. The simulation involves two Local Area Networks (LANs), each connected to a central router. The topology and node arrangement are visualized using NetAnim.

## **Topology Overview**

The network consists of:

- **LAN 1**: Contains multiple nodes connected through a CSMA (Carrier Sense Multiple Access) protocol. All nodes are vertically aligned with the same x-coordinate, creating a vertical LAN structure.
- **LAN 2**: Similar to LAN 1, but positioned further along the x-axis, also in a vertical alignment.
- **Routers**: Three routers are positioned between the two LANs, connected to both LAN 1 and LAN 2 using point-to-point (P2P) links. The routers are aligned horizontally in the middle of the two LANs, facilitating communication between them.

The simulation generates traffic between nodes on LAN 1 and LAN 2 using a UDP Echo Client-Server model. This demonstrates basic communication and routing within the network.
![Network Topology](network_topology.png)

## **Key Components**

- **LAN 1 (n1)**: A set of nodes connected using a CSMA channel with 100 Mbps bandwidth and a small delay. These nodes are positioned vertically along the y-axis.
- **LAN 2 (n2)**: Another set of nodes connected in a similar fashion to LAN 1, but placed further along the x-axis, with all nodes also aligned vertically.
- **Routers**: Three routers facilitate communication between LAN 1 and LAN 2 using point-to-point links.

## **Node Positioning**

In the simulation:

- **LAN 1 Nodes**: Positioned along the same x-axis (x = 10.0) with a vertical distance of 10 units between each node.
- **LAN 2 Nodes**: Positioned similarly, but with an x-coordinate of 50.0, creating a clear visual separation from LAN 1.
- **Routers**: Positioned between LAN 1 and LAN 2 at an x-coordinate of 30.0, with their y-coordinates spaced in alignment with the LAN nodes.

The positioning of nodes is managed through the `SetConstantPosition` function, ensuring a clear and structured network layout in the NetAnim visualization tool.

## **Project Features**

- **NS-3 Simulation**: The simulation uses NS-3 to create and run the network. The simulation includes traffic generation and the use of Internet protocols to handle routing.
- **NetAnim Visualization**: Node positions are visualized using NetAnim, an NS-3 tool that helps users see the topology and the flow of packets between nodes.
- **UDP Communication**: The simulation implements a UDP Echo Server on LAN 2 and an Echo Client on LAN 1, simulating network traffic.

## **Running the Simulation**

To run this simulation, follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/yourusername/yourproject.git
    ```

2. Build the simulation using NS-3:

    ```bash
    ./waf configure
    ./waf
    ```

3. Run the simulation:

    ```bash
    ./waf --run "your-script-name"
    ```

4. Visualize the results in NetAnim: After running the simulation, an XML file (`network_animation.xml`) will be generated. You can open this file using NetAnim to view the network topology and observe the communication between nodes.

## **Customizing the Simulation**

The number of nodes in each LAN can be modified by adjusting the `n1` and `n2` parameters in the script:

```cpp
>>>>>>> f5cd34f5a9ccef949b77059035e248f024a36387
CommandLine cmd;
cmd.AddValue ("n1", "Number of LAN 1 nodes", n1);
cmd.AddValue ("n2", "Number of LAN 2 nodes", n2);
cmd.Parse (argc, argv);
<<<<<<< HEAD

Additionally, you can change the traffic patterns, link properties, or application behavior by modifying the code.
Dependencies

    NS-3: Ensure that NS-3 is installed on your machine.
    NetAnim: Required for visualizing the network simulation. Download it from the official NS-3 website.
=======
>>>>>>> f5cd34f5a9ccef949b77059035e248f024a36387
