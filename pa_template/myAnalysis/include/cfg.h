
#ifndef _CFG_H_
#define _CFG_H_
#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include "generic_graph.h"
#include "graph_visual.h"
#include "graph_test.h"
#include "graph_generator.h"

using namespace std;
class CFGNode;  // Forward declaration
class CFGEdge : public GenericEdge<CFGNode> 
{
public:
    CFGEdge(CFGNode* s, CFGNode* d)
        : GenericEdge<CFGNode>(s, d) {}
    
    ~CFGEdge() {}
};

class CFGNode : public GenericNode<CFGEdge> 
{  
public:
    CFGNode(unsigned Id)
        : GenericNode<CFGEdge>(Id) {}
};

class CFG : public GenericGraph<CFGNode, CFGEdge> 
{
public:
    CFG() {} 
    ~CFG() {}
};

class CFGVisual : public GraphVis<CFGNode, CFGEdge, CFG>
{
public:
    CFGVisual(std::string graphName, CFG* graph)
        : GraphVis<CFGNode, CFGEdge, CFG>(graphName, graph) {}
    
    ~CFGVisual() {}
};

class CFGTest : public GraphTest<CFGNode, CFGEdge, CFG>
{
public:
    CFGTest()
        : GraphTest<CFGNode, CFGEdge, CFG>() {}
    
    ~CFGTest() {}

    void runTests()
    {
        // Add your own tests here
        testGraphDump();
        GraphTest::runTests();
    }

private:
    void testGraphDump()
    {
        GraphGenerator<CFGNode, CFGEdge, CFG> generator;
        CFG randomCFG = generator.generateRandomGraph(10);

        CFGVisual cfgVisual("cfg", &randomCFG);
        cfgVisual.witeGraph();
    }
};

#endif 
