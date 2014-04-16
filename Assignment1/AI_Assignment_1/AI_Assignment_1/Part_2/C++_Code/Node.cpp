/**
@Naveen Neelakandan
CSE 4633 - Part 2
This is the implementation file for the node class
*/

#include "Node.h"

//manhattan heuristic(given)
int manhattan(int *state)		
{
	int i;
	int value = 0;
	for(i=0; i<SIZE; i++)
	{
		switch(state[i])
		{
		case 0: {break;}
		case 1: {value += i%X + i/X; break;}
		case 2: {value += abs(i%X - 1) + i/X; break;}
		case 3: {value += abs(i%X - 2) + i/X; break;}
		case 4: {value += abs(i%X - 2) + abs(i/X - 1); break;}
		case 5: {value += abs(i%X - 2) + abs(i/X - 2); break;}
		case 6: {value += abs(i%X - 1) + abs(i/X - 2); break;}
		case 7: {value += i%X + abs(i/X - 2); break;}
		case 8: {value += i%X + abs(i/X - 1); break;}
		}
	}
	return value;
}



//calculates a heurisic value depending on choice of heuristic -- note only manhattan distance is used
int heuristic(int *state)	//state[0...8]
{
	//return tiles_out_place(state);
	return manhattan(state);
	//return manhattan_linear(state);
}

//constructor for node given an array
Node::Node(int arr[SIZE])
{
	for (int i=0;i<SIZE;i++)
	{
		s[i] = arr[i];
		if (arr[i]==0)
			blank = i;
	}
	g = 0;
	h = heuristic (arr);
	par = NULL;

}

//constructor for node given a parent pointer and next move
Node::Node(Node* p,int d)
{
	for (int i=0;i<SIZE;i++)
	{
		s[i] = p->s[i];
		if (p->s[i]==0)
			blank = i;
	}
	int temp = blank;
	switch(d)
	{
		case 0: 
			//up
			blank = blank -3; 
			s[temp] = s[blank]; 
			s[blank] = 0; 
			break;
		case 1: 
			//down
			blank = blank +3; 
			s[temp] = s[blank]; 
			s[blank] = 0;
			break;
		case 2: 
			//left 
			blank = blank -1; 
			s[temp] = s[blank]; 
			s[blank] = 0;
			break;
		case 3: 
			//right
			blank = blank +1; 
			s[temp] = s[blank]; 
			s[blank] = 0;
			break;

	}

	g = p->g + 1;
	h = heuristic (s);
	par = p;
	

}

//method which determines possible moves for a given state
bool* Node::succarr()
{
	bool* su = new bool[4];
	
	if (blank >= X)
		su[0] = 1;          //move up
	else
		su[0] =0;
	
	if (blank <= SIZE-1-X)
		su[1] = 1;          //move down
	else
		su[1] =0;
	
	if (blank % X != 0 )
		su[2] = 1;          //move left
	else
		su[2] =0;
	
	if (blank % X != X-1)
		su[3] = 1;          //move right
	else
		su[3] =0;

	return su;
}

//prints a node
void Node::print()
{
	cout<<endl;
	for (int y=0;y<SIZE;y++)
	{
		cout<<"-";
		cout<<s[y];
	}
	cout<<endl;
}

//trace solution path from goal to start
void Node::trace_path()
{
	print();
	while(par != NULL)
	{
		par->print();
		Node* grandparent = par->par;
		par = grandparent;
	}
	

}


//methods that gives the new position of blank tile given move direction
int Node::newblankpos(int d)
{
	switch (d)
	{
	case 0:
		return blank-3;
		break;
	case 1:
		return blank+3;
		break;
	case 2:
		return blank-1;
		break;
	case 3:
		return blank+1;
		break;
	}
}