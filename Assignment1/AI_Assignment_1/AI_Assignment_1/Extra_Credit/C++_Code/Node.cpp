/**
@Naveen Neelakandan
CSE 4633 - Extra Credit
This is the implementation file for the node class
*/

#include "Node.h"
int g2[SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};


int manhattan(int* state)
{	
	
	int value = 0;

	for(int i=0;i<SIZE;i++)
	{
		int val_at_goal = g2[i];
		if (val_at_goal == 0)
			continue;
		int curr_pos;
		for (int j=0;j<SIZE;j++)
		{
			if (state[j]==val_at_goal)
				curr_pos = j;
		}
	    int r_g,c_g,r_c,c_c;
		r_g = i / Y;
		c_g = i % X;
		r_c = curr_pos / Y;
		c_c = curr_pos % X;
		int hor_dis, vert_dis;
		if ( r_g > r_c )
			vert_dis = r_g - r_c;
		else 
			vert_dis = r_c - r_g;
		if ( c_g > c_c )
			hor_dis = c_g - c_c;
		else 
			hor_dis = c_c - c_g;
		value += (vert_dis+hor_dis);
	}
	
	return value;
}


//calculates a heurisic value depending on choice of heuristic -- note only manhattan distance is used
int heuristic(int *state)	//state[0...8]
{
	//return tiles_out_place(state);
	return manhattan(state);
//	return manhattan_linear(state);
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
			blank = blank -4; 
			s[temp] = s[blank]; 
			s[blank] = 0; 
			break;
		case 1: 
			//down
			blank = blank +4; 
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
		return blank-4;
		break;
	case 1:
		return blank+4;
		break;
	case 2:
		return blank-1;
		break;
	case 3:
		return blank+1;
		break;
	}
}