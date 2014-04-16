#ifndef OPENLIST_H
#define OPENLIST_H
#include<vector>
#include"head.h"

class openlist_t
{
private:
	int n;
	unsigned char min;
	vector<NODE>b[128];
public:
	openlist_t(){n = 0; min = 0xff;}
	void init(){n = 0; min = 0xff;for(int i=0;i<128;i++)b[i].clear();}
	~openlist_t(){for(int i=0;i<128;i++)b[i].clear();}
	char empty(){return(n>0?0:1);}
	void insert(NODE * node);
	NODE deleteMin();
};

#endif