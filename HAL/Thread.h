#ifndef THREAD_H
#define THREAD_H

#ifdef _MSC_VER
#include <windows.h>
__declspec(dllimport) void *__stdcall CreateThread(
    __in_opt  LPSECURITY_ATTRIBUTES lpThreadAttributes,
    __in      SIZE_T dwStackSize,
    __in      LPTHREAD_START_ROUTINE lpStartAddress,
    __in_opt  LPVOID lpParameter,
    __in      DWORD dwCreationFlags,
    __out_opt LPDWORD lpThreadId
    );

typedef unsigned long thread_handle;
inline thread_handle thread_start(unsigned long (__stdcall *f)(void *), void *a) {
	thread_handle tid;
	CreateThread(0, 0, f, a, 0, &tid);
	return tid;
}

template <typename W, typename V>
struct starter {
	W w; V v; starter(W w, V v) : w(w), v(v) {}
	static unsigned long __stdcall start(void *obj) {
		starter<W,V> *ob = (starter<W, V>*)obj;
		((ob->w)->*(ob->v))();
		delete ob;
		return 0;
	}
};

#else
#include "pthread.h"

typedef pthread_t thread_handle;
inline thread_handle thread_start(void *(*f)(void*), void *a) {
	thread_handle t;
	pthread_create(&t, 0, f, a);
	return t;
}

template <typename W, typename V>
struct starter {
	W w; V v; starter(W w, V v) : w(w), v(v) {}
	static void *start(void *obj) {
		starter<W,V> *ob = (starter<W, V>*)obj;
		((ob->w)->*(ob->v))();
		delete ob;
		return 0;
	}
};
#endif

class Thread {
	thread_handle tid;
public:
	Thread() {}
	template <typename T, typename U>
	void start(T t, U u) {
		starter<T, U> *s = new starter<T, U>(t, u);
		tid = thread_start(&starter<T, U>::start, s);
	}
	void stop() {
	}
};

#endif


