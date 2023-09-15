/*footways.cpp*/

//
// A collection of footways / paths in the Open Street Map.
//
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#include "footways.h"
#include "osm.h"
#include "tinyxml2.h"
#include "graph.h"
#include "nodes.h"
#include "dist.h"
#include "node.h"

using namespace std;
using namespace tinyxml2;

//
// readMapFootways
//
// Given an XML document, reads through the document and
// stores all the footways / paths into the given vector.
//
void Footways::readMapFootways(XMLDocument &xmldoc)
{
  XMLElement *osm = xmldoc.FirstChildElement("osm");
  assert(osm != nullptr);

  //
  // Parse the XML document way by way, looking for footways:
  //
  XMLElement *way = osm->FirstChildElement("way");

  while (way != nullptr)
  {
    const XMLAttribute *attr = way->FindAttribute("id");
    assert(attr != nullptr);

    //
    // if this is a footway, collect the node ids and store another
    // footway object in the vector:
    //
    if (osmContainsKeyValue(way, "highway", "footway") ||
        osmContainsKeyValue(way, "area:highway", "footway"))
    {
      long long id = attr->Int64Value();

      Footway footway(id);

      XMLElement *nd = way->FirstChildElement("nd");

      while (nd != nullptr)
      {
        const XMLAttribute *ndref = nd->FindAttribute("ref");
        assert(ndref != nullptr);

        long long id = ndref->Int64Value();

        footway.add(id);

        // advance to next node ref:
        nd = nd->NextSiblingElement("nd");
      }

      this->MapFootways.push_back(footway);
    } // if

    way = way->NextSiblingElement("way");
  } // while

  //
  // done:
  //
}

//
// intersectWithBuilding
//
// Outputs all the footways that intersect --- i.e. share
// a common node --- with Building B.
//
void Footways::intersectWithBuilding(Building &B)
{
  vector<long long> footwayIDs;

  cout << "Footways that intersect:" << endl;

  for (Footway &F : this->MapFootways)
  {
    if (F.intersectWithBuilding(B))
    {
      footwayIDs.push_back(F.ID);
    }
  }

  //
  // did any intersect?
  //
  if (footwayIDs.empty())
  {
    cout << "  None" << endl;
    return;
  }

  //
  // sort the footway ids, if any:
  //
  sort(footwayIDs.begin(), footwayIDs.end(),
       [](long long id1, long long id2)
       {
         if (id1 < id2)
           return true;
         else
           return false;
       });

  //
  // output the pairs of intersections:
  //
  for (long long id : footwayIDs)
  {
    cout << "  Footway " << id << endl;
  }
}

//
// accessors / getters
//
int Footways::getNumMapFootways()
{
  return (int)this->MapFootways.size();
}

//
// buildgraph
// a function that builds a graph with the nodes as vertices
// and footway nodes as edges
//
void Footways::buildGraph(Nodes &nodes, Footways &footwayz, graph &G)
{
  // find the vertices
  nodes.vertfill(G);

  // Now input edges:  Src Dest Weight
  double weight, lat, lon, lat2, lon2;
  bool entrance;
  for (auto futway : footwayz.MapFootways)
  {
    int leng = futway.NodeIDs.size();
    footwayz.edgefill(nodes, G, leng, futway, lat, lat2, lon, lon2, entrance, weight);
  }
}

//
// helper function to buildgraph
//
// fills in the edges of the graph using the footways
//
void Footways::edgefill(Nodes &nodes, graph &G, int &leng, Footway &futway, double &lat, double &lat2, double &lon, double &lon2, bool &entrance, double &weight)
{
  for (int i = 0; i < leng - 1; i++)
  {
    nodes.find(futway.NodeIDs[i], lat, lon, entrance);
    nodes.find(futway.NodeIDs[i + 1], lat2, lon2, entrance);
    double dist = distBetween2Points(lat, lon, lat2, lon2);
    G.addEdge(futway.NodeIDs[i], futway.NodeIDs[i + 1], dist);
    dist = distBetween2Points(lat2, lon2, lat, lon);
    G.addEdge(futway.NodeIDs[i + 1], futway.NodeIDs[i], dist);
  }
}

//
// sanity check
//
// checks to see if the buildgraph is working correctly and
// that the edges are correct
//
void Footways::sanitycheck(Footways &footwayz, Nodes &nodes)
{
  double weight, lat, lon, lat2, lon2;
  bool entrance;
  for (auto futway : footwayz.MapFootways)
  {
    if (futway.ID == 986532630)
    {
      int leng = futway.NodeIDs.size();
      cout << "Graph check of Footway ID 986532630" << endl;
      footwayz.printedges(nodes, leng, futway, lat, lat2, lon, lon2, entrance, weight);
      break;
    }
  }
}

//
// helper function to sanity check
//
// prints out the edges for the footway with the ID 986532630
//
void Footways::printedges(Nodes &nodes, int &leng, Footway &futway, double &lat, double &lat2, double &lon, double &lon2, bool &entrance, double &weight)
{
  for (int i = 0; i < leng - 1; i++)
  {
    nodes.find(futway.NodeIDs[i], lat, lon, entrance);
    nodes.find(futway.NodeIDs[i + 1], lat2, lon2, entrance);
    double dist = distBetween2Points(lat, lon, lat2, lon2);
    cout << "  Edge: (" << futway.NodeIDs[i] << ", " << futway.NodeIDs[i + 1] << ", " << dist << ")" << endl;
    dist = distBetween2Points(lat2, lon2, lat, lon);
    cout << "  Edge: (" << futway.NodeIDs[i + 1] << ", " << futway.NodeIDs[i] << ", " << dist << ")" << endl;
  }
}
