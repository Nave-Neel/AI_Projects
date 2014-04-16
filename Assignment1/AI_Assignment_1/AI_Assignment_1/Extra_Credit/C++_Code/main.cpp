/**
@Naveen Neelakandan
CSE 4633 - Extra Credit
This is the main driver file for the 8 puzzle solver using IDA*
*/

#include"Node.h"
#include<time.h>
#include<stdio.h>
#include<cstdlib>
#include<math.h>
#include<vector>

using namespace std;

#define DEPTH_LIMIT 70

//Initialize goal node
int g[SIZE] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0};
Node goal(g);

// GOAL
// 1 2 3 4
// 5 6 7 8
// 9 10 11 12
// 13 14 15 0

// Random Test State
// 1 7 3 4  
// 6 0 2 10  
// 13 12 15 14
// 11 9  8  5

//Initialize test node
int st[SIZE] = {1,7,3,4,6,0,2,10,13,12,15,14,11,8,9,5};    //solution length = 40
//int st[SIZE] = {9,2,3,6,8,5,12,10,11,1,0,4,13,7,14,15};  /solution length = 36


//Create global variables

double generated = 0;
double expanded = 0;
int soln_length=0;


vector<Node*> stack;


/*
This is the depthLimited search algorithm. It performs a depth-first search to a given depth limit.
The algorithm is a subroutine of IDA*.
*/
int depthLimited(Node n,int limit)
{
    generated += 1;

	//test goal node
    if(n.h == 0)
	{
		soln_length = n.h + n.g;
        return -1;
	}
	
	//test if node f-value exceeds limit
	if(n.h + n.g > limit)
        return (n.h + n.g);

    int bestLimit = DEPTH_LIMIT;
    

	Node *succ;
	bool* sucarr = n.succarr();

    for(int i = 0; i<4; i++)
    {
		//check if a move is possible
		if (sucarr[i] == 1)
		{	
			//parent checking
			if (n.par != NULL)
			{
				if(n.newblankpos(i) == n.par->blank)
					continue;
			}

			//generate new successor node
			
			succ = new Node(&n,i);
			generated += 1;

			
	        int newLimit = depthLimited(*succ, limit);
			
			//if answer found, trace path
			if (newLimit == -1)
			{
            stack.push_back(succ);
            return -1;
			}

        delete succ;

		//set next cost threshold
		if(bestLimit > newLimit)
			bestLimit = newLimit;
        
		}
	}

    return bestLimit;
}



/*
This is the implementation of the IDA* algorithm. The depth is set to the value of the
lowest-cost node that was pruned during the previous iteration. The algorithm terminates 
when a goal state is reached whose total cost dees not exceed the current threshold.
*/
void IDA_star(Node Start)
{	
    int depth = Start.h;
	cout<<"depth "<<depth<<endl;

	//while solution not found
    while(soln_length == 0)
    {
        cout<<depth<<endl;
        generated = 0;
        depth = depthLimited(Start, depth);
        cout<<"Nodes Generated = "<<generated<<endl;
    
		//if soultion found, trace out path and break out of loop
		if(depth == -1)
        {
            cout<<"Solution found!"<<soln_length<<endl;
			for(int f=0;f<stack.size();f++)
				stack[f]->print();
            break;
        }
        
		//if reached depth_limit and no solution, then break
		if(depth == DEPTH_LIMIT)
        {
            cout<<"This puzzle is no solution"<<endl;
            break;
        }
    
	}
}

/*
This function reads in the file input and creates a respective state for each
line in the textfile.
*/
Node Input_puzzle(FILE *fp)
{	
	int arr[SIZE];
	int index;	/* index to tile positions */
	for (index = 0; index < SIZE; index++)		/* for each position */
	{
		fscanf (fp, "%d", &arr[index]);			/* input tile in that position */
	}
	Node *p = new Node(arr); 
	return *p;
}

int main()
{
	
	Node start(st);
	IDA_star(start);


system("pause");
return 0;}

