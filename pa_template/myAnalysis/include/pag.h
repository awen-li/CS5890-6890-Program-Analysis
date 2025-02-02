#ifndef _PAG_H_
#define _PAG_H_

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include "generic_graph.h"
#include "graph_visual.h"
#include "graph_test.h"
#include "graph_generator.h"

using namespace std;

// Forward declarations
class PAGNode;

// PAGEdge: Inherit from GenericEdge with NodeTy = PAGNode.
class PAGEdge : public GenericEdge<PAGNode> 
{
public:
    PAGEdge (PAGNode* s, PAGNode* d)
      : GenericEdge<PAGNode>(s, d) { }
};


// PAGNode: Inherit from GenericNode with EdgeTy = PAGEdge.
// We add a Label field for storing node-specific information.
class PAGNode : public GenericNode<PAGEdge> 
{
public:
    PAGNode (unsigned id)
      : GenericNode<PAGEdge> (id) { }
};


// PAG: Our graph type; inherit from GenericGraph using PAGNode and PAGEdge.
class PAG : public GenericGraph<PAGNode, PAGEdge> 
{
public:
    PAG () {} 
    ~PAG () {}
};



// PAGVis: A DOT generator for our PAG (constraint graph).
// Inherits from the template GraphVis using our PAG types.
class PAGVis: public GraphVis<PAGNode, PAGEdge, PAG>
{
public:
    PAGVis (string graphName, PAG *graph)
        : GraphVis<PAGNode, PAGEdge, PAG>(graphName, graph) {}

    ~PAGVis () {}
};



// PAGTest: Test cases for our PAG (constraint graph).
// Inherits from the template GraphTest using our PAG types.
class PAGTest: public GraphTest<PAGNode, PAGEdge, PAG>
{
public:
    PAGTest () : GraphTest<PAGNode, PAGEdge, PAG> () {}
    ~PAGTest () {}

    void runTests ()
    {
        // Add your own test here.
        PAGtest1 ();

        GraphTest<PAGNode, PAGEdge, PAG>::runTests();
    }

private:
    void PAGtest1 ()
    {
        PAG pag;
        
        PAGNode* node1 = new PAGNode(1, "Node 1");
        PAGNode* node2 = new PAGNode(2, "Node 2");
        PAGNode* node3 = new PAGNode(3, "Node 3");
        
        pag.addNode(1, node1);
        pag.addNode(2, node2);
        pag.addNode(3, node3);
        
        PAGEdge* edge1 = new PAGEdge(node1, node2);
        PAGEdge* edge2 = new PAGEdge(node2, node3);
        PAGEdge* edge3 = new PAGEdge(node1, node3);
        
        pag.addEdge(edge1);
        pag.addEdge(edge2);
        pag.addEdge(edge3);
        
        PAGVis vis("PAGTestGraph", &pag);
        vis.witeGraph();
    }
};

#endif
