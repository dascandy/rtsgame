#include "swap.h"

void swap(unsigned int &a, unsigned int &b)
{
	unsigned int t = a;
	a = b;
	b = t;
}

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

void swap(bool &a, bool &b)
{
	bool t = a;
	a = b;
	b = t;
}

