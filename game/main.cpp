#include "Game.h"
#include "ResourceFile.h"
#include <SDL.h>
#include "Scene.h"
#include "debug.h"
#include "profile.h"

#ifdef _MSC_VER
#include <windows.h>

int main(int, char *[])
{
	profile_init();
	char cwd[1024];
	int chars = GetCurrentDirectory(1023, cwd);
	cwd[chars] = 0;

	char resdir[1024];
	sprintf(resdir, "%s\\res\\", cwd);
	int rval = 0;
	ResourceFile::Instance(resdir);
	try
	{
		Game().run();
	}
	catch(std::exception &e) 
	{
		MessageBox(0, e.what(), "Exception!", 0);
	}
	catch(int rv)
	{
		rval = rv;
	}
	profile_fini();
	return rval;
}
#else
int main(int, char *[])
{
	profile_init();

	int rval = 0;
	ResourceFile::Instance("./");
	try
	{
		Game().run();
	}
	catch(std::exception &e) 
	{
		printf("FATAL exception %s\n", e.what());
	}
	catch(int rv)
	{
		rval = rv;
	}
	profile_fini();
	return rval;
}
#endif


