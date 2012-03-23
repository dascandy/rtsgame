#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include "Dll.h"
#include "Thread.h"

class HALSPEC ThreadPool {
	std::vector<Thread *> threads;
public:
	template <typename T, typename U>
	ThreadPool(int count, T t, U u) {
		for (int i = 0; i < count; i++) {
			Thread *thr = new Thread();
			thr->start(t, u);
			threads.push_back(thr);
		}
	}
};

#endif

