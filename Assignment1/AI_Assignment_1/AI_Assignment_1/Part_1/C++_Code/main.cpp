/*
Goal: 

1 2 3 
8   4 
7 6 5 
*/
#include"openlist.h"
#include"hashtable.h"
#include"head.h"
#include<time.h>
#include<stdio.h>
#include<cstdlib>
#include<math.h>
#include<conio.h>
using namespace std;
NODE goal = {1,2,3,8,0,4,7,6,5,4,0,0};	//the last 3 elements are for goal.blank, g, h

NODE start;

double generated = 0;
double expanded = 0;
double Total_Generated = 0;
double Total_Expanded = 0;

openlist_t OL;		//open list
hash_table CL;		//close list

int tiles_out_place(char *state)
{
	int i;
	int value = 0;
	for(i=0; i<SIZE; i++)
	{
		if(state[i] != goal.s[i])
			value++;
	}
	return value;
}

int manhattan(char *state)		//this function is only used for reaching the Goal state shown above
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

/*
This is the manhattan distance with linear conflict heuristic. +2 is added for every pair of tiles
in their correct row/column but have to pass each other to get to goal state
*/
int manhattan_linear(char* state)
 {	
	//compute manhattan distance
	int total = 0;
	total = manhattan(state);

	int row;
	vector<int> goal_v;
	
	//for each row determine the linear conflicts
	for (row = 0; row <X; row++)
	{
		goal_v.clear();

		//determine elements in correct row
		for (int i=0;i<X;i++)
		{
			for (int j=0;j<X;j++)
			{
				if (state[i+row*3] == goal.s[j+row*3])
				{	if(int(state[i+row*3]) !=0 )
						goal_v.push_back(goal.s[j+row*3]);
				
				}
			}
		}
		
		if (goal_v.size()>1)
		{	
			vector<int> goal_pos;
			
			//evaluate goal positions for each element in correct row 
			for (int i=0; i<int(goal_v.size()); i++)
			{
				for (int z= row*3; z<(row+1)*3; z++)
				{
					if (goal_v[i] == goal.s[z])
						{	
							goal_pos.push_back(z);
						}
				}
			}

			//determine if elements need to pass each other to reach goal, if yes add 2
			for(int i=0;i<int(goal_pos.size());i++)
			{
				for(int j=i+1;j<int(goal_pos.size());j++)
				{
					if (goal_pos[i]>goal_pos[j])
					{	
						
						total += 2;
						break;
					}
				}
			}
		
		}

	}

	int col;

	//for each column determine the linear conflicts
	for (col = 0; col <Y; col++)
	{
		goal_v.clear();

		//determine elements in correct column
		for (int i=0;i<X;i++)
		{
			for (int j=0;j<X;j++)
			{
				if (state[3*i+col] == goal.s[3*j+col])
				{	if(int(state[3*i+col]) != 0)
					goal_v.push_back(goal.s[3*j+col]);
				}
			}
		}
		
		if (goal_v.size()>1)
		{	
			
			vector<int> goal_pos2;

			//evaluate goal positions for each element in correct column
			for (int i=0; i<int(goal_v.size()); i++)
			{
				for (int z=col; z<=(X-1)*Y+col; z+=3)
				{
					if (goal_v[i] == goal.s[z])
						{	
							goal_pos2.push_back(z);
						}
				}
			}

			//determine if elements need to pass each other to reach goal, if yes add 2
			for(int i=0;i<int(goal_pos2.size());i++)
			{
				for(int j=i+1;j<int(goal_pos2.size());j++)
				{
					if (goal_pos2[i]>goal_pos2[j])
					{	

						total += 2;
						break;
					}
				}
			}
		
		}

	}

	
	return total;
 }


int heuristic(char *state)	//state[0...8]
{
	//return tiles_out_place(state);
	//return manhattan(state);
	return manhattan_linear(state);
	
}


void puzzle_init(NODE * state)		//set the position of blank tile and g cost
{
	int i;
	for(i=0; i<SIZE; i++)
		if(state->s[i] == 0)
		{
			state->blank = i;
			break;
		}
	state->g = 0;
}

struct move_t
{
	int num;		/* number of applicable oprs: 2..4 */
	int pos[4];		/* position of adjacent tiles for each position */
}oprs[SIZE];		//This data structure stores all the possible moves according to the possition of blank tile

void InitMove()
{
	int blank;		/* possible positions of blank */
	for(blank = 0; blank < SIZE; blank++)	/* for each possible blank position */
	{
		oprs[blank].num = 0;	/* no moves initially */
		if(blank >= X)			/* not top edge */
			oprs[blank].pos[oprs[blank].num++] = blank - X;	/* add a move up */
		if(blank < SIZE - X)	/* not bottom edge */
			oprs[blank].pos[oprs[blank].num++] = blank + X;	/* add a move down */
		if(blank % X > 0)		/* not left edge */
			oprs[blank].pos[oprs[blank].num++] = blank - 1;	/* add a move left */
		if(blank % X < X - 1)	/* not right edge */
			oprs[blank].pos[oprs[blank].num++] = blank + 1;	/* add a move right */
	}
}

void trace_path(NODE * CurrentNode)
{
	if(CurrentNode->g == 0)		//start node
		return;
	NODE best,ChildNode;
	best.g = 127;	//CHAR_MAX
	for(int i=0; i<oprs[CurrentNode->blank].num; i++)
	{
		ChildNode = *CurrentNode;
		ChildNode.blank = oprs[CurrentNode->blank].pos[i];
		ChildNode.s[CurrentNode->blank] = CurrentNode->s[ChildNode.blank];
		ChildNode.s[ChildNode.blank] = 0;
		if((ChildNode.g = CL.find_g(&ChildNode)) >= 0)
		{
			if(ChildNode.g < best.g)
				best = ChildNode;
		}
	}
	trace_path(&best);
	cout<<"step "<<(int)CurrentNode->g<<":"<<endl;
	for(int i = 0;i<Y;i++)
	{
		for(int j=0; j<X; j++)
		{
			printf("%d ",CurrentNode->s[j + i*X]);
		}
		putchar('\n');
	}
}

char astar(NODE * start)
{
	NODE CurrentNode,ChildNode;
	OL.insert(start);
	while(!OL.empty())
	{
		expanded++;
		CurrentNode = OL.deleteMin();
		CL.insert(&CurrentNode);		//put into close list
		//expand child nodes
		for(int i=0; i<oprs[CurrentNode.blank].num; i++)	//for each possible move, generate child node
		{
			ChildNode = CurrentNode;
			ChildNode.blank = oprs[CurrentNode.blank].pos[i];
			ChildNode.s[CurrentNode.blank] = CurrentNode.s[ChildNode.blank];
			ChildNode.s[ChildNode.blank] = 0;
			ChildNode.g++;
			ChildNode.h = heuristic(ChildNode.s);
			if(ChildNode.h==0)
			{
				//cout<<"Solution Found!"<<endl;
				//trace_path(&ChildNode);
				goal.g = ChildNode.g;
				return 1;
			}
			if(!CL.find(&ChildNode))		//This child node is not in closelist, put it into openlist
			{
				generated++;
				OL.insert(&ChildNode);
			}
		}

	}
	cout<<"This puzzle does not have a solution."<<endl;
	return 0;
}

NODE Input_puzzle(FILE *fp)
{
	NODE state;
	int index;	/* index to tile positions */
	for (index = 0; index < SIZE; index++)		/* for each position */
	{
		fscanf (fp, "%d", &state.s[index]);			/* input tile in that position */
	}
	state.g = 0;
	return state;
}

char s[SIZE] = {6,2,3,8,0,5,7,1,4};

int main()
{	


	int problem;					 /* problem instance */
	FILE *fp;
	clock_t begin, finish;
	double runtime,Total_time = 0;
	InitMove();
	fp = fopen("test.txt","r");
	if(!fp)
	{
		cout<<"Cannot open input file!\n"<<endl;
		return 0;
	}
	for (problem = 1; problem <= 40; problem++)
	{
		generated = expanded = 0;
		OL.init();
		CL.init();
		start = Input_puzzle(fp);
		begin = clock();
		puzzle_init(&start);
		start.h = heuristic(start.s);
		if(start.h == 0)
		{
			cout<<"The start state is the goal.";
		}
	
		astar(&start);
		finish = clock();
		runtime = (double)(finish - begin)/CLOCKS_PER_SEC;
		cout<<"Steps:"<<(int)goal.g;
		cout<<" Time:"<<runtime<<" s.";
		cout<<" Nodes generated:"<<generated;
		cout<<", expanded:"<<expanded<<endl;
		Total_Generated += generated;
		Total_Expanded += expanded;
		Total_time += runtime;
	}
	cout<<"Average time:"<<Total_time/(problem-1)<<" s.";
	cout<<"Average nodes generated:"<<Total_Generated/(problem-1);
	cout<<", expanded:"<<Total_Expanded/(problem-1)<<endl;
	fclose(fp);
	return 0;
}



