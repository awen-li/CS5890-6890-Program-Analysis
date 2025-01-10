#ifndef _GENERIC_GRAPH_H_
#define _GENERIC_GRAPH_H_



template<class NodeTy> class GenericEdge 
{    
private:
    NodeTy* m_SrcNode;
    NodeTy* m_DstNode;

public:

    GenericEdge(NodeTy* s, NodeTy* d): m_SrcNode(s),m_DstNode(d)
    {

    }

    virtual ~GenericEdge() 
    {
    }

    inline bool operator== (const GenericEdge<NodeTy>* rhs) const 
    {
        return (rhs->GetAttr () == this->GetAttr () &&
                rhs->GetSrcID() == this->GetSrcID() &&
                rhs->GetDstID() == this->GetDstID());
    }

    typedef struct 
    {
        bool operator()(const GenericEdge<NodeTy>* lhs, const GenericEdge<NodeTy>* rhs) const 
        {
            if (lhs->GetSrcID() != rhs->GetSrcID())
            {
                return lhs->GetSrcID() < rhs->GetSrcID();
            }
            else
            {
                return lhs->GetDstID() < rhs->GetDstID();
            }
        }
    } EqualGEdge;
};


template<class EdgeTy> class GenericNode 
{

public:
    typedef std::set<EdgeTy*, typename EdgeTy::EqualGEdge> T_GEdgeSet;
    typedef typename T_GEdgeSet::iterator iterator;

private:
    unsigned m_Id;

    T_GEdgeSet m_InEdgeSet;  
    T_GEdgeSet m_OutEdgeSet;  

public:
    GenericNode(unsigned Id): m_Id(Id) 
    {
    }

    virtual ~GenericNode() 
    {        
        Release();
    }

    inline void Release()
    {
        for (auto In = InEdgeBegin (), End = InEdgeEnd (); In != End; In++)
        {
            RmIncomingEdge(*In);      
        }
        m_InEdgeSet.clear();      
        
        for (auto In = OutEdgeBegin (), End = OutEdgeEnd (); In != End; In++)
        {
            RmOutgoingEdge(*In);      
        }
        m_OutEdgeSet.clear();
    }

    inline iterator OutEdgeBegin()
    {
        return m_OutEdgeSet.begin();
    }
    
    inline iterator OutEdgeEnd() 
    {
        return m_OutEdgeSet.end();
    }
    
    inline iterator InEdgeBegin() 
    {
        return m_InEdgeSet.begin();
    }
    
    inline iterator InEdgeEnd() 
    {
        return m_InEdgeSet.end();
    }

    inline bool AddIncomingEdge(EdgeTy* InEdge)
    {
        return m_InEdgeSet.insert(InEdge).second;
    }
    
    inline bool AddOutgoingEdge(EdgeTy* OutEdge) 
    {
        return m_OutEdgeSet.insert(OutEdge).second;
    }

    inline void RmIncomingEdge(EdgeTy* InEdge) 
    {
        iterator it = m_InEdgeSet.find(InEdge);
        if(it == m_InEdgeSet.end())
        {
            return;
        }

        m_InEdgeSet.erase(InEdge);
        return;
    }
    
    inline void RmOutgoingEdge(EdgeTy* OutEdge) 
    {
        iterator it = m_OutEdgeSet.find(OutEdge);
        if(it == m_OutEdgeSet.end())
        {
            return;
        }

        m_OutEdgeSet.erase(OutEdge);
        return;
    }
};

template<class NodeTy,class EdgeTy> class GenericGraph 
{

public:
    typedef llvm::DenseMap<unsigned, NodeTy*> T_IDToNodeMap;   
    typedef typename T_IDToNodeMap::iterator node_iterator;

protected:
    unsigned m_NodeNum;
    unsigned m_EdgeNum;
    T_IDToNodeMap m_IDToNodeMap;
    
public:
    
    GenericGraph()
    {
        m_NodeNum = 0;
        m_EdgeNum = 0;
    }

    virtual ~GenericGraph()
    {       
        for (auto I = m_IDToNodeMap.begin(), E = m_IDToNodeMap.end(); I != E; ++I)
        {
            delete I->second;
        }      
    }

    inline node_iterator begin() 
    {
        return m_IDToNodeMap.begin();
    }
    
    inline node_iterator end() 
    {
        return m_IDToNodeMap.end();
    }

    inline void AddNode(unsigned id, NodeTy* node) 
    {
        m_IDToNodeMap[id] = node;
        m_NodeNum++;
    }

    inline void RmNode(NodeTy* Node) 
    {
        
    }

    inline bool AddEdge(EdgeTy* Edge)
    {
        if (Edge->GetDstNode()->AddIncomingEdge(Edge))
        {
            Edge->GetSrcNode()->AddOutgoingEdge(Edge);
            m_EdgeNum++;

            return true;
        }

        return false;
    }

    inline void RmEdge(EdgeTy* Edge)
    {
        Edge->GetDstNode()->RmIncomingEdge(Edge);
        Edge->GetSrcNode()->RmOutgoingEdge(Edge);
            
        m_EdgeNum--;
        delete Edge;
        return;
    }
};

#endif 
