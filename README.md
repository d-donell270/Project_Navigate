# Project_Navigate

This is a modern C++ program to perform basic navigation of an Open Street Map, 
it focuses on the area around Northwestern’s Evanston campus, and the app is to find the shortest path between buildings. 
It focuses on two features: “Nodes” and “Ways”. 
A node is a point on the map, consisting of 3 values: id, latitude, and longitude. 
A way is a series of nodes that define something. The two most important examples in our case are buildings and footways. 
For a building, the nodes define the building’s perimeter. 
For a footway, the nodes define the endpoints of the footway, and typically include intermediate points along the way (especially if the footway is not a straight line).

Run program with the command: ./a.out and open the loaded map: nu.osm and follow instructions that follow to get the needed information.
