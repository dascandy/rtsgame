#include "os.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

msec_t get_msec()
{
	return GetTickCount();
}
/*
void sleep(msec_t time) 
{
	Sleep((DWORD)time);
}
*/
#else
#include <sys/time.h>
#include <unistd.h>

msec_t get_msec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	msec_t ctime = (msec_t)(tv.tv_sec * 1000) + tv.tv_usec / 1000;
	return ctime;
}
/*
void sleep(msec_t time) 
{
	usleep(time*1000);
}
*/
#endif

