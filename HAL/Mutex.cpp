#include "Mutex.h"
#include <windows.h>

Mutex::Mutex() 
: h(CreateMutex(0, false, 0))
{
}

void Mutex::get() {
	while (WaitForSingleObject(h, INFINITE) != WAIT_OBJECT_0) {}
}

void Mutex::put() {
	ReleaseMutex(h);
}

Mutex::~Mutex() {
	CloseHandle(h);
}

