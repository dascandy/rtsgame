#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

// including windows.h breaks my code due to it containing evil defines
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
extern "C" void Log(const char *fmt, ...);

#ifdef _MSC_VER
#define FLPREFIX "%s(%d): "
#else
#define FLPREFIX "%s:%d: "
#endif

const char *basename(const char *name);
extern int zero;
#define TODO(x, ...) do { Log(FLPREFIX "TODO: " x "\nFATAL\n", basename(__FILE__), __LINE__, __VA_ARGS__); DebugBreak(); } while (zero)
#define TODO_W(x, ...) do { static bool beenHere = false; if (!beenHere) { Log(FLPREFIX "TODO: " x "\n", basename(__FILE__), __LINE__, __VA_ARGS__); beenHere = true; } } while (zero)
#define LOG_WARN(f, ...) do { Log(FLPREFIX "WARN: " f "\n", basename(__FILE__), __LINE__, __VA_ARGS__); } while(zero)
#define ERR(x, ...) do { Log(FLPREFIX "ERR : " x "\nFATAL\n", basename(__FILE__), __LINE__, __VA_ARGS__); DebugBreak(); } while(zero)

#endif

