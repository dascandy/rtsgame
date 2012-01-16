#include "Random.h"
#include <stdlib.h>
#include "debug.h"

unsigned int Random::getNextI(unsigned int max)
{
	TODO_W("Implement proper random impl");
	return rand() % max;
}

