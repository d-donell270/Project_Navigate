/*main.cpp*/

//
// Program to input Nodes (positions), Buildings and Footways
// from an Open Street Map file.
//
// Prof. Joe Hummel
// Northwestern University
// CS 211: Winter 2023
//

#include <iostream>
#include <string>
#include <iomanip>
#include <assert.h>
#include <queue>
#include <map>
#include <cmath>
#include <stack>

#include "dist.h"
#include "building.h"
#include "buildings.h"
#include "footway.h"
#include "footways.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "graph.h"

using namespace std;
using namespace tinyxml2;

class prioritize
{
public:
  bool operator()(pair<long long, double> &U1, pair<long long, double> &U2) const
  {
    if (U1.second > U2.second)
      return true;
    else if (U1.second < U2.second)
      return false;
    else
    {
      return U1.first > U2.first;
    }
  }
};

//
// Dijkstra:
//
// Performs Dijkstra's shortest weighted path algorithm from
// the given start vertex.
//
map<long long, long long> Dijkstra(graph &G, long long startV, long long endV, map<long long, double> &distances)
{
  vector<long long> visited;
  map<long long, long long> predv;
  set<long long> vset;

  priority_queue<pair<long long, double>, vector<pair<long long, double>>, prioritize> unvisitedQ;

  for (auto &cur : G.getVertices())
  {
    if (cur != startV)
    {
      unvisitedQ.push(make_pair(cur, INFINITY));
      distances.emplace(cur, INFINITY);
      predv.emplace(cur, 0);
    }
    else
    {
      unvisitedQ.push(make_pair(cur, (double)0));
      distances.emplace(cur, (double)0);
    }
  }

  while (unvisitedQ.size() != 0)
  {
    long long cur = unvisitedQ.top().first;
    unvisitedQ.pop();
    if (distances[cur] == INFINITY)
      break;
    else if (vset.find(cur) != vset.end())
      continue;
    else
    {
      visited.push_back(cur);
      vset.emplace(cur);
    }

    for (auto adjv : G.neighbors(cur))
    {
      double weight;
      bool bweight = G.getWeight(cur, adjv, weight);
      if (!bweight)
        cout << "**Edge has no weight" << endl;
      double altdist = distances[cur] + weight;

      if (altdist < distances[adjv])
      {
        unvisitedQ.push(make_pair(adjv, altdist));
        distances[adjv] = altdist;
        predv[adjv] = cur;
      }
    }
  }

  cout << "Shortest weighted path:" << endl;
  if (distances[endV] != INFINITY){
  cout << "  # nodes visited: " << visited.size() << endl;
  cout << "  Distance: " << distances[endV] << " miles" << endl;

  stack<long long> path;
  path.push(endV);
  long long cur = endV;
  while (cur != startV)
  {
    cur = predv[cur];
    path.push(cur);
  }

  cout << "  Path: ";
  while (!path.empty())
  {
    if (path.size() != 1)
    {
      cout << path.top() << "->";
      path.pop();
    }
    else
    {
      cout << path.top() << endl;
      path.pop();
    }
  }
  }
  else {
    cout << "**Sorry, destination unreachable" << endl;
  }
  return predv;
}

//
// ldist
//
void ldist(Footways &footwayz, pair<double, double> avgs, Nodes &nodes, long long &fid)
{
  long long id = 0;
  double lat = 0, lon = 0;
  bool entr = false;
  double lst;
  double lst2 = INFINITY;
  Footway cloze(id);
  Node intr(id, lat, lon, entr);

  for (auto futway : footwayz.MapFootways)
  {
    for (auto nod : futway.NodeIDs)
    {
      nodes.find(nod, lat, lon, entr);
      lst = distBetween2Points(avgs.first, avgs.second, lat, lon);
      if (lst < lst2)
      {
        lst2 = lst;
        id = nod;
        cloze = futway;
      }
    }
  }

  fid = id;
  cout << "  Closest footway ID " << cloze.ID << ", node ID " << id << ", distance " << lst2 << endl;
  return;
}

//
// navigate()
//
void navigate(Buildings &buildings, Nodes &nodes, Footways &footwayz, graph &G)
{

  string sname, dname, noname = "", noadd = "";
  long long noid = 0, sid, did;
  Building startB(noid, noname, noadd);
  Building DestB(noid, noname, noadd);
  map<long long, double> distances;
  std::cout << "Enter start building name (partial or complete)> " << endl;
  getline(cin, sname);
  bool found = false;
  for (auto B : buildings.MapBuildings)
  {
    if (B.Name.find(sname) == string::npos)
      continue;
    else
    {
      found = true;
      assert(found == true);
      cout << "  Name: " << B.Name << endl;
      pair<double, double> savgs = B.getLocation(nodes);
      cout << "  Approximate location: (" << savgs.first << ", " << savgs.second << ")" << endl;
      startB = B;
      ldist(footwayz, savgs, nodes, sid);
      break;
    }
  }

  if (found == false)
  {
    cout << "**Start building not found" << endl;
    return;
  }

  std::cout << "Enter destination building name (partial or complete)> " << endl;
  getline(cin, dname);
  found = false;
  for (auto B : buildings.MapBuildings)
  {
    if (B.Name.find(dname) == string::npos)
      continue;
    else
    {
      found = true;
      assert(found == true);
      cout << "  Name: " << B.Name << endl;
      pair<double, double> davgs = B.getLocation(nodes);
      cout << "  Approximate location: (" << davgs.first << ", " << davgs.second << ")" << endl;
      DestB = B;
      ldist(footwayz, davgs, nodes, did);
      break;
    }
  }

  if (found == false)
  {
    cout << "**Destination building not found" << endl;
    return;
  }
  else
  {
    map<long long, long long> predv = Dijkstra(G, sid, did, distances);
  }
}

//
// main
//
int main()
{
  cout << setprecision(12);
  XMLDocument xmldoc;
  Nodes nodes;
  Buildings buildings;
  Footways footways;
  graph G;

  cout << "** NU open street map **" << endl;

  string filename;

  cout << endl;
  cout << "Enter map filename> " << endl;
  getline(cin, filename);

  //
  // 1. load XML-based map file
  //
  if (!osmLoadMapFile(filename, xmldoc))
  {
    // failed, error message already output
    return 0;
  }

  //
  // 2. read the nodes, which are the various known positions on the map:
  //
  nodes.readMapNodes(xmldoc);

  //
  // NOTE: let's sort so we can use binary search when we need
  // to lookup nodes.
  //
  nodes.sortByID();

  //
  // 3. read the university buildings:
  //
  buildings.readMapBuildings(xmldoc);

  //
  // 4. read the footways, which are the walking paths:
  //
  footways.readMapFootways(xmldoc);

  //
  // 5. create vertices and edges:
  //
  footways.buildGraph(nodes, footways, G);

  //
  // 6. stats
  //
  cout << "# of nodes: " << nodes.getNumMapNodes() << endl;
  cout << "# of buildings: " << buildings.getNumMapBuildings() << endl;
  cout << "# of footways: " << footways.getNumMapFootways() << endl;
  cout << "# of graph vertices: " << G.NumVertices() << endl;
  cout << "# of graph edges: " << G.NumEdges() << endl;

  //
  // now let the user for search for 1 or more buildings:
  //
  while (true)
  {
    string name;

    cout << endl;
    cout << "Enter building name, * to list, @ to navigate, or $ to end> " << endl;

    getline(cin, name);

    if (name == "$")
    {
      break;
    }
    else if (name == "*")
    {
      buildings.print();
    }
    else if (name == "!")
    {
      footways.sanitycheck(footways, nodes);
    }
    else if (name == "@")
    {
      navigate(buildings, nodes, footways, G);
    }
    else
    {
      buildings.findAndPrint(name, nodes, footways);
    }

  } // while

  //
  // done:
  //
  cout << endl;
  cout << "** Done  **" << endl;
  cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
  cout << "# of Nodes created: " << Node::getCreated() << endl;
  cout << "# of Nodes copied: " << Node::getCopied() << endl;
  cout << endl;

  return 0;
}
