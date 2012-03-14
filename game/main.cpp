#include "Game.h"
#include <SDL/SDL.h>
#include "debug.h"
#include "profile.h"
#include "os.h"

extern "C" int main(int, char *argv[])
{
	platInit();
	int rval = 0;
	profile_init(argv[0]);
	try
	{
		Game().run();
	}
	catch(std::exception &e) 
	{
		Fatal(e.what());
	}
	catch(int rv)
	{
		rval = rv;
	}
	profile_fini();
	return rval;
}

