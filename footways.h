/*footways.h*/

//
// A collection of footways / paths in the Open Street Map.
//
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
//

#pragma once

#include <vector>

#include "building.h"
#include "footway.h"
#include "tinyxml2.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

//
// Keeps track of all the footways / paths in the map.
//
class Footways
{
public:
  vector<Footway> MapFootways;

  //
  // readMapFootways
  //
  // Given an XML document, reads through the document and
  // stores all the footways into the given vector.
  //
  void readMapFootways(XMLDocument &xmldoc);

  //
  // intersectWithBuilding
  //
  // Outputs all the footways that intersect --- i.e. share
  // a common node --- with Building B.
  //
  void intersectWithBuilding(Building &B);

  //
  // accessors / getters
  //
  int getNumMapFootways();

  //
  // buildgraph
  // a function that builds a graph with the nodes as vertices
  // and footway nodes as edges
  //
  void buildGraph(Nodes &nodes, Footways &footwayz, graph &G);

  //
  // helper function to buildgraph
  //
  // fills in the edges of the graph using the footways
  //
  void edgefill(Nodes &nodes, graph &G, int &leng, Footway &futway, double &lat, double &lat2, double &lon, double &lon2, bool &entrance, double &weight);

  //
  // sanity check
  //
  // checks to see if the buildgraph is working correctly and
  // that the edges are correct
  //
  void sanitycheck(Footways &footwayz, Nodes &nodes);

  //
  // helper function to sanity check
  //
  // prints out the edges for the footway with the ID 986532630
  //
  void printedges(Nodes &nodes, int &leng, Footway &futway, double &lat, double &lat2, double &lon, double &lon2, bool &entrance, double &weight);
};
