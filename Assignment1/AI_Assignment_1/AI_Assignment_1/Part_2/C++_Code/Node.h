/**
@Naveen Neelakandan
CSE 4633 - Part 2
This is the header file for the node class
*/

#ifndef NODE_H
#define NODE_H

#include<iostream>
#include<vector>
#define X 3
#define Y 3
#define SIZE (X*Y)
using namespace std;



class Node
{
public:
	int s[SIZE];
	int blank;		//position of blank tile
	int g,h;
	Node* par;
	Node(int arr[SIZE]);
	Node(Node*p,int d);
	bool* succarr();
	void print();
	void trace_path();
	int newblankpos(int);
};





#endif;
