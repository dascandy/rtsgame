#ifndef MUTEX_H
#define MUTEX_H

#include "Dll.h"

class HALSPEC Mutex {
	void * h;
public:
	Mutex();
	void get();
	void put();
	~Mutex();
};

#endif

