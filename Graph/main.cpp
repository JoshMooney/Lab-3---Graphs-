
#include <iostream>
#include <fstream>

#include "Graph.h"

using namespace std;


typedef GraphArc<char, int> Arc;
typedef GraphNode<char, int> Node;

void visit( Node * pNode ) {
	cout << "Visiting: " << pNode->data() << endl;
}

int main(int argc, char *argv[]) {
   
    Graph<char, int> graph( 9 );
	
	char c = 0;
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
	graph.breadthFirst( graph.nodeArray()[0], visit );

	system("PAUSE");
	
}
