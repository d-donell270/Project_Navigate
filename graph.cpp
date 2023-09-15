/*graph.cpp */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "graph.h"

using namespace std;

//
// default constructor: no parameters
//
graph::graph()
{
}

//
// NumVertices
//
// Returns the # of vertices currently in the graph.
//

int graph::NumVertices() const
{
    return this->Vertices.size();
}

//
// NumEdges
//
// Returns the # of edges currently in the graph.
//
int graph::NumEdges() const
{
    return this->EdgeCount;
}

//
// addVertex
//
// Adds the vertex v to the graph and returns true. However,
// if v is already in the graph, it is not added again and
// false is returned.
//
bool graph::addVertex(string v)
{
    if (Vertextoindex.find(v) == Vertextoindex.end())
    {
        int syz = this->Vertices.size();
        this->Vertices.emplace_back(v);
        this->Vertextoindex.emplace(v, syz);
        this->Edges.push_back({});
        return true;
    }
    else
    {
        return false;
    }
}

//
// addEdge
//
// Adds the edge (from, to, weight) to the graph, and returns
// true. If the vertices do not exist then no edge is added
// and false is returned.
//
// NOTE: if the edge already exists, the existing edge weight
// is overwritten with the new edge weight.
//
bool graph::addEdge(string from, string to, double weight)
{
    if ((Vertextoindex.find(from) == Vertextoindex.end()) || Vertextoindex.find(to) == Vertextoindex.end())
    {
        return false;
    }
    else
    {
        int numfrom = this->Vertextoindex[from];
        int numto = this->Vertextoindex[to];
        // bool already = false;

        // for (auto &pos : this->Edges[numfrom])
        // {
        //     if (pos.first == numto)
        //     {
        //         pos.second = weight;
        //         already = true;
        //         break;
        //     }
        // }

        if (Edges[numfrom].find(numto) == Edges[numfrom].end()) {
            EdgeCount ++;
        }
        this->Edges[numfrom][numto] = weight;

        return true;
    }
}


//
// getWeight
//
// Returns the weight associated with a given edge.  If
// the edge exists, the weight is returned via the reference
// parameter and true is returned. If the edge does not
// exist, the weight parameter is unchanged and false is
// returned.
//
bool graph::getWeight2(string from, string to, double &weight) const
{   // Vertextoindex.find(from) == Vertextoindex.end() DOES NOT EXISTS
    if ((Vertextoindex.find(from) == Vertextoindex.end()) || (Vertextoindex.find(to) == Vertextoindex.end()))
    {
        return false;
    }
    else
    {
        int numfrom = this->Vertextoindex.at(from);
        int numto = this->Vertextoindex.at(to);
        if (Edges[numfrom].find(numto) != Edges[numfrom].end()){
            weight = Edges[numfrom].at(numto);
            return true;
        }
        return false;
    }
}

bool graph::getWeight(string from, string to, double &weight) const
{
    bool yeah = getWeight2(from, to, weight);
    return yeah;
}

//
// neighbors
//
// Returns a set containing the neighbors of v, i.e. all
// vertices that can be reached from v along one edge. If
// v does not exist, an empty set is returned.
//
set<string> graph::neighbors2(string v) const
{
    set<string> neybahs = {};
    if (Vertextoindex.find(v) == Vertextoindex.end())
    {
        return neybahs;
    }
    else
    {
        int numfrom = this->Vertextoindex.at(v);
        for (auto pos : Edges[numfrom])
        {
            neybahs.emplace(Vertices[pos.first]);
        }
    }
    return neybahs;
}


set<string> graph::neighbors(string v) const
{
    set<string> nya;
    nya = neighbors2(v);
    return nya;
}

//
// getVertices
//
// Returns a vector containing all the vertices currently in
// the graph. There is no guaranteed order to the vertices
// in the returned vector; the order should be considered
// random.
//
vector<string> graph::getVertices() const
{
    return this->Vertices;
}

//
// print
//
// Prints the internal state of the graph to the given
// output stream for debugging purposes. The output will
// contain the # of vertices, the # of edges, the vertices,
// and the edges. Vertices are output as strings and the
// weights as doubles.
//
// Example:
//    graph G;
//    ...
//    G.print(cout);  // print to console
//
void graph::print(ostream &output) const
{
    cout << "# of vertices: " << NumVertices() << endl
         << "# of edges: " << NumEdges() << endl;
    cout << "The Vertices : " << endl;
    for (auto vert : this->Vertices)
    {
        cout << "  " << vert << endl;
    }
    cout << "_E_D_G_E_S_(  from  to  weight)" << endl;
    int count = 0;
    string fro, too;
    for (auto from : this->Edges)
    {
        count++;
        for (auto fname : Vertextoindex)
        {
            if (fname.second == count)
            {
                fro = fname.first;
                break;
            }
        }
        for (auto to : from)
        {
            for (auto fname : Vertextoindex)
            {
                if (fname.second == to.first)
                {
                    too = fname.first;
                    break;
                }
            }
            cout << "  " << fro << "  " << too << "  " << to.second << endl;
        }
    }
}
