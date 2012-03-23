#ifndef QUEUEDWORK_H
#define QUEUEDWORK_H

#include <deque>
#include <vector>
#include "Mutex.h"
#include "Semaphore.h"
#include "ThreadPool.h"
#include "Dll.h"

class HALSPEC Queued {
public:
	virtual ~Queued() {}
	virtual void run() = 0;
};

class HALSPEC QueuedWork {
	Mutex lock, poll;
	Semaphore semaphore;
	std::deque<Queued *> MayBlock, OpenGLContext;
	std::vector<Queued *> Polls;
	ThreadPool pool;
	QueuedWork();
	static QueuedWork &Instance() { static QueuedWork inst; return inst; }
	void RunBlocking();
public:
	enum Type { Blocking, OpenGL, Polling };
	static void Run(Type what, int maxcount);
	static void Queue(Queued *what, Type type);
};

#endif

