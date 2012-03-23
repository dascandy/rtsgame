#ifndef COHERENT_H
#define COHERENT_H

class Coherent {
	unsigned int seed1, seed2;
	int levels;
	double getInputValue(int x, int y);
	double getLayerValue(double x, double y);
public:
	Coherent(int levels, unsigned int seed1, unsigned int seed2);
	double getValue(double x, double y);
};

#endif

