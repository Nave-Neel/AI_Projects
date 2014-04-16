#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include<vector>
#include"head.h"

#define HASH_SIZE 2000000
//using namespace std;

class hash_table
{
private:
	int n;
	int collision;
	vector<NODE>table[HASH_SIZE];
public:
	hash_table(){n=collision=0;}
	int hash(NODE * node);		//return hash value
	int insert(NODE * node);
	char find_g(NODE * node);
	bool find(NODE * node);		//1 for exists in hash table, 0 for not exists or better state
	void print_usage(){cout<<"Hash table usage: elements:"<<n<<" collision:"<<collision<<endl;};
	void init(){n=collision=0;for(int i=0;i<HASH_SIZE;i++)table[i].clear();};
};

#endif