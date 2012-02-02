#include "debug.h"
#include <stdarg.h>

// To prevent compiler warning C4121 to tell us "while(0) is constant" in macros
int zero = 0;
/*
const char *basename(const char *name)
{
	const char *lastBackslash = name;
	while (*name)
	{
		++name;
		if (*name == '\\')
			lastBackslash = name;
	}
	return lastBackslash + 1;
}
*/
void Log(const char *text, ...)
{
	char target[256];
	va_list args;
	va_start(args, text);
	vsnprintf(target, 255, text, args);
	va_end(args);

	printf("%s", target);
#ifdef _MSC_VER
	OutputDebugStringA(target);
#endif
}

