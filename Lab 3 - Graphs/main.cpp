#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;


typedef GraphArc<int, int> Arc;
typedef GraphNode<int, int> Node;

void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data() << endl;
}

int main(int argc, char *argv[]) {
   
	Graph<int, int> graph(25);
	
	int c = 0;
	int i = 0;
	ifstream myfile;
	myfile.open ("nodes.txt");

	while ( myfile >> c ) {
		graph.addNode(c, i++);
	}

	myfile.close();
	myfile.open("arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();

	// Now traverse the graph.
	//graph.breadthFirst( graph.nodeArray()[0], visit );

	Node *target = graph.nodeArray()[15];
	Node *origin = graph.nodeArray()[0];
	graph.breadthFirstSearch(origin, target, visit);

	Node* current = target;
	while (current != origin)
	{
		visit(current);
		current = current->getPreviousNode();
	}
	visit(origin);
	system("PAUSE");
	
}
