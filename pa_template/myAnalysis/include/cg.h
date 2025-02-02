
#ifndef _CG_H_
#define _CG_H_
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include "generic_graph.h"
#include "graph_visual.h"
#include "graph_test.h"
#include "graph_generator.h"

using namespace std;

class CGNode;  // Forward declaration
class CGEdge : public GenericEdge<CGNode> 
{
public:
    CGEdge(CGNode* s, CGNode* d)
        : GenericEdge<CGNode>(s, d) {}
    
    ~CGEdge() {}
};

class CGNode : public GenericNode<CGEdge> 
{  
public:
    CGNode(unsigned Id)
        : GenericNode<CGEdge>(Id) {}
};

class CG : public GenericGraph<CGNode, CGEdge> 
{
public:
    CG() {} 
    ~CG() {}
};

class CGVisual : public GraphVis<CGNode, CGEdge, CG>
{
public:
    CGVisual(std::string graphName, CG* graph)
        : GraphVis<CGNode, CGEdge, CG>(graphName, graph) {}
    
    ~CGVisual() {}
};

class CGTest : public GraphTest<CGNode, CGEdge, CG>
{
public:
    CGTest() 
        : GraphTest<CGNode, CGEdge, CG>() {}
    
    ~CGTest() {}

    void runTests()
    {
        // Add your own tests here
        testGraphDump();
        GraphTest::runTests();
    }

private:
    void testGraphDump()
    {
        GraphGenerator<CGNode, CGEdge, CG> generator;
        CG randomGraph = generator.generateRandomGraph(10);

        CGVisual cgVisual("cg", &randomGraph);
        cgVisual.witeGraph();
    }
};


#endif 
