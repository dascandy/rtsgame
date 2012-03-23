#include "Semaphore.h"
#include <windows.h>

Semaphore::Semaphore(int count) 
: h(CreateSemaphore(0, 0, count, 0))
{
}

void Semaphore::get() {
	while (WaitForSingleObject(h, INFINITE) != WAIT_OBJECT_0) {}
}

void Semaphore::put() {
	ReleaseSemaphore(h, 1, 0);
}

Semaphore::~Semaphore() {
	CloseHandle(h);
}

