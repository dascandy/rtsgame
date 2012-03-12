#include "os.h"
#include <stdio.h>
#include "ResourceManager.h"

static void webinit();

#ifdef _WIN32
#include <windows.h>

msec_t get_msec()
{
	return GetTickCount();
}

void delay(msec_t time) 
{
	Sleep((DWORD)time);
}

void platInit() {
	char cwd[1024];
    int chars = GetCurrentDirectory(1023, cwd);
    cwd[chars] = 0;
    char resdir[1024];
    sprintf(resdir, "%s\\res\\", cwd);
    ResourceManager::Instance().setRootDir(resdir);
	webinit();
}

void Fatal(const char *error) {
	MessageBox(0, error, "Fatal error!", 0);
}

#else
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

msec_t get_msec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	msec_t ctime = (msec_t)(tv.tv_sec * 1000) + tv.tv_usec / 1000;
	return ctime;
}

void delay(msec_t time) 
{
	usleep(time*1000);
}

void platInit() {
    ResourceManager::Instance().setRootDir("/home/pebi/projects/rtsgame/rtsgame/game/res/");
	webinit();
}

void Fatal(const char *error) {
	printf(0, error, "Fatal error!", 0);
	abort();
}

#endif

#include "Textures.h"
#include "WebProfile.h"
#include "Webserver.h"
#include "WebShader.h"
#include "QueuedWork.h"
#include "WebFont.h"
#include "WebRoot.h"
#include "WebRenderPass.h"

class QueuedWebserverPoll : public Queued {
public:
	void run() { Webserver::Instance().Poll(); }
};

static void webinit() {
	web_init_root();
	web_init_renderpass();
	web_init_textures();
	web_init_profile();
	web_init_shader();
	web_init_font();
	QueuedWork::Polls.push_back(new QueuedWebserverPoll());
}

