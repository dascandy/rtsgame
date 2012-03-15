#include "Random.h"
#include <stdlib.h>
#include "debug.h"

Random::Random(int seed) {
	TODO_W("Implement proper random impl");
}

unsigned int Random::getNextI(unsigned int max)
{
	unsigned int largenum = rand() + 65536 * rand();
	return largenum % max;
}

double Random::getNextD() {
	return (getNextI(4294967295) / 4294967296.0 + getNextI(4294967295)) / 4294967296.0;
}

