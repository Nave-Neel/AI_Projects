/**
@Naveen Neelakandan
CSE 4633 - Part 2
This is the main driver file for the 8 puzzle solver using IDA*
*/

#include"Node.h"
#include<time.h>
#include<stdio.h>
#include<cstdlib>
#include<math.h>
using namespace std;

// GOAL
// 1 2 3
// 8   4
// 7 6 5

//Initialize goal node
int g[SIZE] = {1,2,3,8,0,4,7,6,5};
Node goal(g);	

// Random Test State
// 7 4 2   
//   3 5   
// 8 6 1   

//Initialize test node
int st[SIZE] = {8,0,2,7,1,3,4,5,6};
Node start(st);

//Create global variables
int bestlimit;
double generated = 0;
double expanded = 0;
int soln_length=0;

/*
This is the depth_limited search algorithm. It performs a depth-first search to a given depth limit.
The algorithm is a subroutine of IDA*.
*/
int depth_limited(Node n, int l)
{	
	//check if solution is already found, if yes then exit
	if (soln_length != 0)
		return NULL;

	//create successor pointer and array of possible moves
	Node *succ;
	bool* sucarr = n.succarr();
	
	//for each possible move of tile
	for (int i=0 ;i<4 ;i++)
	{	
		//check if a move is possible
		if (sucarr[i] == 1)
		{	
			//parent checking
			if (n.par != NULL){
			if(n.newblankpos(i) == n.par->blank)
				continue;
			}

			//generate new successor node
			succ = new Node(&n,i);
			generated += 1;
			
			//check if we have reached goal node
			if(succ->h==0)
			{
				cout<<"Solution Found!"<<endl;
				goal.g = succ->g;
				soln_length = goal.g;
				break;
				//succ->trace_path();         //comment this out to see solution path
			}

			//check if successor f-value exceeds limit
			if (succ->g + succ->h > l)
			{	
				//max number of moves
				if (succ->g + succ->h > 32)
					continue;

				//set lowest cost threshold
				if (succ->g + succ->h  < bestlimit)
				{
					bestlimit = succ->g + succ->h; 
					continue;
				}
			}

			//successor value does not exceed limit; expand successor node
			else
			{	expanded += 1;
				depth_limited(*succ,l);
			}
		}

	}
return 0;
}


/*
This is the implementation of the IDA* algorithm. The depth is set to the value of the
lowest-cost node that was pruned during the previous iteration. The algorithm terminates 
when a goal state is reached whose total cost dees not exceed the current threshold.
*/
int IDA_star(Node start)
{	
	//initialize limit
	int newlimit = start.h;

	//while solution is not found
	while (soln_length == 0)
	{
		cout<<" Depth: "<<newlimit<<endl;
		if (newlimit > 32)
		{
			cout<<"THIS PUZZLE HAS NO SOLUTION"<<endl;
			break;

		}

		//initialize variables at each iteration
		generated = 0;
		expanded = 0;
		bestlimit = 1000000;

		//call to depth_limited search function
		depth_limited(start,newlimit);
		
		//update cost threshold 
		newlimit = bestlimit;

		cout<<" Generated: "<< generated<<endl;
		cout<<" Expanded: "<< expanded<<endl;
		cout<<endl;
	}
	return soln_length;


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


//Main Function
int main()
{
	//initialize variables
	int problem;
	FILE *fp;
	clock_t begin, finish;
	double runtime,Total_time = 0;

	//create file handle
	if(!(fp = fopen("test.txt","r")))
	{
		cout<<"Cannot open input file!\n"<<endl;
		return 0;
	}


	for (problem = 1; problem <= 20; problem++)
	{	
		cout<<"Problem: "<<problem<<endl;
		start = Input_puzzle(fp);
		
		//initialize variables before each problem
		generated = 0;
		expanded = 0;
		soln_length = 0;
		bestlimit=10000000;
		
		//Solve and calculate runtime
		begin = clock();
		IDA_star(start);
		finish = clock();
		runtime = (double)(finish - begin)/CLOCKS_PER_SEC;
	
		cout<<"Runtime: "<<runtime<<endl;
		cout<<"Solution Length: "<<soln_length<<endl;
		cout<<"----------------------------------"<<endl;
		cout<<endl;
	}
		fclose(fp);
		system("pause");
		return 0;
}
