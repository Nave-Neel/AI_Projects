#ifndef HEAD_H
#define HEAD_H

#include<iostream>
#include<vector>
#define X 3
#define Y 3
#define SIZE (X*Y)
using namespace std;

typedef struct node_t
{
	char s[SIZE];
	char blank;		//position of blank tile
	char g,h;
}NODE;

#endif
