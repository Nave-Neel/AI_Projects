#include"openlist.h"


void openlist_t::insert(NODE * node)
{
	int f = node->g + node->h;
	if(f >= 128 || f < 0)
		return;
	n++;
	if(f < min)
		min = f;
	b[f].push_back(*node);
}

NODE openlist_t::deleteMin()
{
	NODE x;
	x = b[min].back();
	b[min].pop_back();
	n--;
	if(n>0)
	{
		while(b[min].empty() && min<128)
		{
			min++;
		} 
	}
	else
		min = 0xff;	//MAX
	return x;
}