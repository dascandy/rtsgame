#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "Dll.h"

class HALSPEC Semaphore {
	void * h;
public:
	Semaphore(int count);
	void get();
	void put();
	~Semaphore();
};

#endif

