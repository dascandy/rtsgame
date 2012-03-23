#include "QueuedWork.h"

QueuedWork::QueuedWork()
: semaphore(50)
, pool(8, this, &QueuedWork::RunBlocking) 
{
}

void QueuedWork::RunBlocking() {
	Run(Blocking, -1);
}

void QueuedWork::Run(Type what, int maxcount) {
	switch (what) {
		case Polling:
			Instance().poll.get();
			for (std::vector<Queued *>::iterator it = Instance().Polls.begin(); it != Instance().Polls.end(); ++it) {
				(*it)->run();
			}
			Instance().poll.put();
			break;
		case Blocking:
			for (;;) {
				Instance().semaphore.get();
				Instance().lock.get();
				Queued *q = Instance().MayBlock.front();
				Instance().MayBlock.pop_front();
				Instance().lock.put();
				q->run();
			}
			break;
		case OpenGL:
			Instance().lock.get();
			for (int i = 0; i < maxcount && Instance().OpenGLContext.size(); i++) {
				Queued *q = Instance().OpenGLContext.front();
				Instance().OpenGLContext.pop_front();
				Instance().lock.put();
				q->run();
				Instance().lock.get();
			}
			Instance().lock.put();
			break;
	}
}

void QueuedWork::Queue(Queued *what, Type type) {
	switch(type) {
		case Blocking: 
			Instance().lock.get();
			Instance().MayBlock.push_back(what);
			Instance().semaphore.put();
			Instance().lock.put();
			break;
		case OpenGL:
			Instance().lock.get();
			Instance().OpenGLContext.push_back(what);
			Instance().lock.put();
			break;
		case Polling:
			Instance().poll.get();
			Instance().Polls.push_back(what);
			Instance().poll.put();
			break;
	}
}

