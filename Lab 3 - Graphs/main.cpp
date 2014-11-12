#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>

#include "Graph.h"

using namespace std;


typedef GraphArc<string, int> Arc;
typedef GraphNode<string, int> Node;

void visit( Node * pNode ) 
{
	cout << "Visiting: " << pNode->data() << endl;
}

int main(int argc, char *argv[]) {
   
	Graph<pair<string,int>, int> graph(6);
	
	pair<string,int> c;
	int i = 0;
	ifstream myfile;
	myfile.open ("nodes.txt");

	while ( myfile >> c.first ) 
	{
		graph.addNode(c, i++);
	}

	myfile.close();
	myfile.open("arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) 
	{
		graph.addArc(from, to, weight);
	}
    myfile.close();

	Node *target = graph.nodeArray()[15];
	Node *origin = graph.nodeArray()[0];

	Node* current = target;
	while (current != origin)
	{
		visit(current);
		current = current->getPreviousNode();
	}
	visit(origin);
	system("PAUSE");
	
}
