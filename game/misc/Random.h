#ifndef RANDOM_H
#define RANDOM_H

class Random
{
public:
	Random(int seed);
	unsigned int getNextI(unsigned int max);
	float getNextF();
	double getNextD();
};

#endif

