#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <string>

#include "Graph.h"

using namespace std;


typedef GraphArc<string, int> Arc;
typedef GraphNode<pair<string, int>, int> Node;

void Nothing(Node * pNode)	{}

void visit(Node * pNode)
{
	cout << "Visiting: " << pNode->getData().first << endl;
}
void pathfinding(Node * pNode)
{
	cout << "Path: " << pNode->getData().first << endl;
}
void AllRoutes(vector<Node *> pNode)
{
	for (int i = 0; i < pNode.size() - 1; i++)
	{
		cout << "Journey: " << pNode.at(i)->getData().first << " (" << pNode.at(i)->getData().second << ")"<< endl;
	}
	cout << endl;
}

void MovePathToMap()
{

}

int main(int argc, char *argv[])
{

	Graph<pair<string, int>, int> graph(6);

	pair<string, int> c = pair<string, int>("", INT_MAX);
	int i = 0;
	ifstream myfile;
	myfile.open("DorNodes.txt");

	while (myfile >> c.first)
	{
		graph.addNode(c, i++);
	}

	myfile.close();
	myfile.open("DorArcs.txt");

	int from, to, weight;
	while (myfile >> from >> to >> weight)
	{
		graph.addArc(from, to, weight);
	}
	myfile.close();


	vector<Node *> path;

	for (int i = 0; i < 5; i++)
	{
		for (int j = i + 1; j <= 5; j++)
		{
			path.clear();
			graph.UCS(graph.nodeArray[i], graph.nodeArray[j], path, Nothing);
			MovePathToMap();
		}
	}




	system("PAUSE");
}