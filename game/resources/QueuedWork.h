#ifndef QUEUEDWORK_H
#define QUEUEDWORK_H

#include <deque>
#include <vector>

class Queued {
public:
	virtual ~Queued() {}
	virtual void run() = 0;
};

class QueuedWork {
public:
	static std::deque<Queued *> ResourceLoading, Background;
	static std::vector<Queued *> Polls; 
};

#endif


