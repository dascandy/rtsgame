#ifndef THREAD_H
#define THREAD_H

#include "pthread.h"

class Thread {
public:
	template <typename W, typename V>
	struct starter {
		W w; V v; starter(W w, V v) : w(w), v(v) {}
		static void *start(void *obj) {
			((starter<W, V>*)obj)->run();
			return 0;
		}
		void run() { (w->*v)(); }
	};

	pthread_t tid;
	Thread() {}
	template <typename T, typename U>
	void start(T t, U u) {
		starter<T, U> *s = new starter<T, U>(t, u);
		pthread_create(&tid, 0, &starter<T, U>::start, s);
	}
	void stop() {
	}
};

#endif


