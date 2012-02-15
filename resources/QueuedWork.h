#ifndef QUEUEDWORK_H
#define QUEUEDWORK_H

#include <vector>

class Queued {
public:
	virtual ~Queued() {}
	virtual void run() = 0;
};

class QueuedWork {
	static std::vector<Queued *> ResourceLoading, Background, Polls; 
};

#endif


