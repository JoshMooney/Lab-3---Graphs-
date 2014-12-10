#ifndef GRAPH_H
#define GRAPH_H

#include <list>
#include <queue>

using namespace std;

template <class NodeType, class ArcType> class GraphArc;
template <class NodeType, class ArcType> class GraphNode;

// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    This is the graph class, it contains all the
//                  nodes.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
class Graph {
private:

    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

// ----------------------------------------------------------------
//  Description:    An array of all the nodes in the graph.
// ----------------------------------------------------------------
    Node** m_pNodes;

// ----------------------------------------------------------------
//  Description:    The maximum number of nodes in the graph.
// ----------------------------------------------------------------
    int m_maxNodes;


// ----------------------------------------------------------------
//  Description:    The actual number of nodes in the graph.
// ----------------------------------------------------------------
    int m_count;


public:           
    // Constructor and destructor functions
    Graph( int size );
    ~Graph();

    // Accessors
    Node** nodeArray() const {
       return m_pNodes;
    }

    // Public member functions.
    bool addNode( NodeType data, int index );
    void removeNode( int index );
	bool addArc(int from, int to, ArcType weight);
	bool addDualArc(int from, int to, ArcType weight);
    void removeArc( int from, int to );
    Arc* getArc( int from, int to );
	void prepUCS();
    void clearMarks();
    void depthFirst( Node* pNode, void (*pProcess)(Node*) );
	void breadthFirst(Node* pNode, void(*pProcess)(Node*));
	void breadthFirstPlus(Node* pNode, Node* pTarget, void(*pProcess)(Node*));
	void UCS(Node* pStart, Node* pTarget, void(*pProcess)(Node*), std::vector<Node*>& path);

};

template<class NodeType, class ArcType>
class NodeSearchCostComparer {

public:
	bool operator()(GraphNode<NodeType, ArcType> * n1, GraphNode<NodeType, ArcType> * n2) {
		return n1->data().second > n2->data().second;
	}
};


// ----------------------------------------------------------------
//  Name:           Graph
//  Description:    Constructor, this constructs an empty graph
//  Arguments:      The maximum number of nodes.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::Graph( int size ) : m_maxNodes( size ) {
   int i;
   m_pNodes = new Node * [m_maxNodes];
   // go through every index and clear it to null (0)
   for( i = 0; i < m_maxNodes; i++ ) {
        m_pNodes[i] = 0;
   }

   // set the node count to 0.
   m_count = 0;
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
Graph<NodeType, ArcType>::~Graph() {
   int index;
   for( index = 0; index < m_maxNodes; index++ ) {
        if( m_pNodes[index] != 0 ) {
            delete m_pNodes[index];
        }
   }
   // Delete the actual array
   delete m_pNodes;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode( NodeType data, int index ) {
   bool nodeNotPresent = false;
   // find out if a node does not exist at that index.
   if ( m_pNodes[index] == 0) {
      nodeNotPresent = true;
      // create a new node, put the data in it, and unmark it.
      m_pNodes[index] = new Node;
      m_pNodes[index]->setData(data);
      m_pNodes[index]->setMarked(false);

      // increase the count and return success.
      m_count++;
    }
        
    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode( int index ) {
     // Only proceed if node does exist.
     if( m_pNodes[index] != 0 ) {
         // now find every arc that points to the node that
         // is being removed and remove it.
         int node;
         Arc* arc;

         // loop through every node
         for( node = 0; node < m_maxNodes; node++ ) {
              // if the node is valid...
              if( m_pNodes[node] != 0 ) {
                  // see if the node has an arc pointing to the current node.
                  arc = m_pNodes[node]->getArc( m_pNodes[index] );
              }
              // if it has an arc pointing to the current node, then
              // remove the arc.
              if( arc != 0 ) {
                  removeArc( node, index );
              }
         }
        

        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_pNodes[index];
        m_pNodes[index] = 0;
        m_count--;
    }
}

// ----------------------------------------------------------------
//  Name:           addArc
//  Description:    Adds an arc from the first index to the 
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc( int from, int to, ArcType weight ) {
     bool proceed = true; 
     // make sure both nodes exist.
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         proceed = false;
     }
        
     // if an arc already exists we should not proceed
     if( m_pNodes[from]->getArc( m_pNodes[to] ) != 0 ) {
         proceed = false;
     }

     if (proceed == true) {
        // add the arc to the "from" node.
        m_pNodes[from]->addArc( m_pNodes[to], weight );
		cout << "Adding arc from " << m_pNodes[from]->data().first << " to " << m_pNodes[to]->data().first << " weight " << weight << endl;
     }
        
     return proceed;
}

template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addDualArc(int from, int to, ArcType weight) {
	bool proceed = true;
	// make sure both nodes exist.
	if (m_pNodes[from] == 0 || m_pNodes[to] == 0) {
		proceed = false;
	}

	// if an arc already exists we should not proceed
	if (m_pNodes[from]->getArc(m_pNodes[to]) != 0) {
		proceed = false;
	}

	if (proceed == true) {
		// add the arc to the "from" node.
		m_pNodes[from]->addArc(m_pNodes[to], weight);
		m_pNodes[to]->addArc(m_pNodes[from], weight);
		//cout << "Adding dual arc from " << m_pNodes[from]->data().first << " to " << m_pNodes[to]->data().first << " weight " << weight << endl;
	}

	return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc( int from, int to ) {
     // Make sure that the node exists before trying to remove
     // an arc from it.
     bool nodeExists = true;
     if( m_pNodes[from] == 0 || m_pNodes[to] == 0 ) {
         nodeExists = false;
     }

     if (nodeExists == true) {
        // remove the arc.
        m_pNodes[from]->removeArc( m_pNodes[to] );
     }
}

// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
// Dev-CPP doesn't like Arc* as the (typedef'd) return type?
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc( int from, int to ) {
     Arc* pArc = 0;
     // make sure the to and from nodes exist
     if( m_pNodes[from] != 0 && m_pNodes[to] != 0 ) {
         pArc = m_pNodes[from]->getArc( m_pNodes[to] );
     }
                
     return pArc;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::prepUCS()
{
	cout << "////===== Prepping UCS" << endl;
	int index;
	for (index = 0; index < m_maxNodes; index++) {
		if (m_pNodes[index] != 0) {
			m_pNodes[index]->setData(pair<string, int>(m_pNodes[index]->data().first, INT_MAX));
			//cout << m_pNodes[index]->data().first << " distance maxed." << endl;
			m_pNodes[index]->setMarked(false);
			m_pNodes[index]->setPrev(NULL);
			//cout << m_pNodes[index]->data().first << " unmarked" << endl;
		}
	}

	//cout << endl;
}

// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks() {
     int index;
     for( index = 0; index < m_maxNodes; index++ ) {
          if( m_pNodes[index] != 0 ) {
			  m_pNodes[index]->setMarked(false);
			  cout << m_pNodes[i]->data().first << " unmarked." << endl;
          }
     }
}

// ----------------------------------------------------------------
//  Name:           depthFirst
//  Description:    Performs a depth-first traversal on the specified 
//                  node.
//  Arguments:      The first argument is the starting node
//                  The second argument is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::depthFirst( Node* pNode, void (*pProcess)(Node*) ) {
     if( pNode != 0 ) {
           // process the current node and mark it
           pProcess( pNode );
           pNode->setMarked(true);

           // go through each connecting node
           list<Arc>::iterator iter = pNode->arcList().begin();
           list<Arc>::iterator endIter = pNode->arcList().end();
        
		   for( ; iter != endIter; ++iter) {
			    // process the linked node if it isn't already marked.
                if ( (*iter).node()->marked() == false ) {
                   depthFirst( (*iter).node(), pProcess);
                }            
           }
     }
}

// ----------------------------------------------------------------
//  Name:           breadthFirst
//  Description:    Performs a depth-first traversal the starting node
//                  specified as an input parameter.
//  Arguments:      The first parameter is the starting node
//                  The second parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirst( Node* pNode, void (*pProcess)(Node*) ) {
   if( pNode != 0 ) {
	  queue<Node*> nodeQueue;        
	  // place the first node on the queue, and mark it.
      nodeQueue.push( pNode );
      pNode->setMarked(true);

      // loop through the queue while there are nodes in it.
      while( nodeQueue.size() != 0 ) {
         // process the node at the front of the queue.
         pProcess( nodeQueue.front() );

         // add all of the child nodes that have not been 
         // marked into the queue
         list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
         list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();
         
		 for( ; iter != endIter; iter++ ) {
              if ( (*iter).node()->marked() == false) {
				 // mark the node and add it to the queue.
                 (*iter).node()->setMarked(true);
                 nodeQueue.push( (*iter).node() );
              }
         }

         // dequeue the current node.
         nodeQueue.pop();
      }
   }  
}

// ----------------------------------------------------------------
//  Name:           breadthFirstPlus
//  Description:    Performs a depth-first traversal from the starting node
//                  to the target node specified as input parameters.
//  Arguments:      The first parameter is the starting node
//					The second parameter is the target node
//                  The third parameter is the processing function.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::breadthFirstPlus(Node* pNode, Node* pTarget, void(*pProcess)(Node*)) {
	if (pNode != 0) {
		queue<Node*> nodeQueue;
		bool found = false;
		// place the first node on the queue, and mark it.
		nodeQueue.push(pNode);
		pNode->setMarked(true);

		// loop through the queue while there are nodes in it.
		while (nodeQueue.size() != 0 && !found) {
			// process the node at the front of the queue.
			pProcess(nodeQueue.front());

			// add all of the child nodes that have not been 
			// marked into the queue
			list<Arc>::const_iterator iter = nodeQueue.front()->arcList().begin();
			list<Arc>::const_iterator endIter = nodeQueue.front()->arcList().end();

			for (; iter != endIter && !found; iter++) {
				//if the node is our target set found to true
				if ((*iter).node() == pTarget)
				{
					(*iter).node()->setPrev(nodeQueue.front());
					found = 1;
				}
				//else add it to the queue
				else if ((*iter).node()->marked() == false) {
					// mark the node and add it to the queue.
					(*iter).node()->setMarked(true);
					(*iter).node()->setPrev(nodeQueue.front());
					nodeQueue.push((*iter).node());
				}
			}

			// dequeue the current node.
			nodeQueue.pop();
			
		}
	}
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::UCS(Node* pStart, Node* pTarget, void(*pProcess)(Node*), std::vector<Node*>& path)
{
	//init distances and unmark
	prepUCS();
	pStart->setData(pair<string, int>(pStart->data().first, 0));

	cout << "////===== UCS from " << pStart->data().first << " to " << pTarget->data().first << endl;

	//make & set up queue
	priority_queue<Node*, vector<Node*>, NodeSearchCostComparer<NodeType, ArcType>> pq;
	
	//Start of UCS
	pq.push(pStart);
	pStart->setMarked(true);
	
	//Priority Queueue loop
	while (!pq.empty() && pq.top() != pTarget)
	{

		//cout << "TOP: " << pq.top()->data().first << endl;

		//for each child node
		list<Arc>::const_iterator iter = pq.top()->arcList().begin();
		list<Arc>::const_iterator endIter = pq.top()->arcList().end();
	
		//Process all children of the top node
		for (; iter != endIter; iter++) {

			//if the previous node is not top of the queue
			if ((*iter).node() != pq.top()->getPrev())
			{
				//cout << "Checking: " << pq.top()->data().first << " -> " << (*iter).node()->data().first << " ";

				//Get total weight of this route
				int c = pq.top()->getArc((*iter).node())->weight() + pq.top()->data().second;

				//cout << "[" << c << " < " << ((*iter).node()->data().second) << "]" << endl;

				//if it's lower than the weight of the current route
				if (c < ((*iter).node()->data().second))
				{
					//cout << "True, " << (*iter).node()->data().first << " weight is now " << c << ", previous is now " << pq.top()->data().first << endl;

					//Set the node's internal weight to the arc from previous plus internal weight of previous
					(*iter).node()->setData(pair<string, int>((*iter).node()->data().first, c));
					//Set previous pointer of the node to the previous node in the new path
					(*iter).node()->setPrev(pq.top());
				}

				else
				{
					//if ((*iter).node()->getPrev() != 0)
						//cout << "False, " << (*iter).node()->data().first << " remaining " << ((*iter).node()->data().second) << ", previous remains " << (*iter).node()->getPrev()->data().first << endl;
					//else cout << "False, " << (*iter).node()->data().first << " remaining " << ((*iter).node()->data().second) << ", previous remains NULL" << endl;
				}

				//if not marked
				if ((*iter).node()->marked() == false) {
					//add it to the queue and mark
					pq.push((*iter).node());
					//cout << "Queueing:  " << (*iter).node()->data().first << endl;
					(*iter).node()->setMarked(true);
				}
			}
		}
		//cout << "Popping: " << pq.top()->data().first << endl << endl;
		pq.pop();
	}
	
	//Add the nodes to path
	while (pTarget->getPrev() != NULL)
	{
		path.push_back(pTarget);
		pTarget = pTarget->getPrev();
	}
	path.push_back(pTarget);
}

#include "GraphNode.h"
#include "GraphArc.h"


#endif
