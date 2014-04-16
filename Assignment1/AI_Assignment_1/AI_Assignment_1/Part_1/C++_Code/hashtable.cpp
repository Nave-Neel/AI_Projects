#include"hashtable.h"
using namespace std;

int hash_table::hash(NODE * node)
{
	unsigned int pos = 0;
	for(int i=0; i<SIZE; i++)
	{
		pos = pos * 31 + node->s[i] + '0';
	}
	pos %= HASH_SIZE;
	return pos;
}

int hash_table::insert(NODE * node)
{
	unsigned int pos = hash(node);
	if(table[pos].empty())
	{
		table[pos].push_back(*node);
		n++;
		return 1;
	}
	else	//in hash table or collision
	{
		for(unsigned int i=0; i<table[pos].size(); i++)		//check if this node is already in hash table
		{
			int j;
			for(j=0; j<SIZE; j++)
			{
				if(table[pos][i].s[j] != node->s[j])
					break;
			}
			if(j == SIZE)		//the same state exists
			{
				if(node->g < table[pos][i].g)		//change to a better state
				{
					table[pos][i].g = node->g;
					return 2;
				}
				return 0;		//already in hash table
			}
		}
		//not in hash table
		collision++;
		n++;
		table[pos].push_back(*node);
		return 1;
	}
}
char hash_table::find_g(NODE * node)
{
	int pos = hash(node);
	if(table[pos].empty())		//not in hash table
		return -1;
	else	//in or collision
	{
		for(unsigned int i=0; i<table[pos].size(); i++)		//check if this node is already in hash table
		{
			int j;
			for(j=0; j<SIZE; j++)
			{
				if(table[pos][i].s[j] != node->s[j])
					break;
			}
			if(j == SIZE)		//the same state exists
			{
				return table[pos][i].g;		//already in hash table
			}
		}
		//not in hash table
		return -1;
	}
}
bool hash_table::find(NODE * node)
{
	int pos = hash(node);
	if(table[pos].empty())
		return 0;
	else	//in or collision
	{
		for(unsigned int i=0; i<table[pos].size(); i++)		//check if this node is already in hash table
		{
			int j;
			for(j=0; j<SIZE; j++)
			{
				if(table[pos][i].s[j] != node->s[j])
					break;
			}
			if(j == SIZE)		//the same state exists
			{
				if(node->g < table[pos][i].g)		//a better state
					return 0;
				return 1;		//already in hash table
			}
		}
		//not in hash table
		return 0;
	}
}