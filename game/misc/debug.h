#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
// including windows.h breaks my code due to it containing evil defines
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
extern "C" void Log(const char *fmt, ...);

#define FLPREFIX "%s(%d): "
#else
inline void DebugBreak() {}
extern "C" void Log(const char *fmt, ...);

#define FLPREFIX "%s:%d: "
#endif

extern int zero;
#ifdef _MSC_VER
#define TODO(x, ...) do { Log(FLPREFIX "TODO: " x "\nFATAL\n", basename(__FILE__), __LINE__, __VA_ARGS__); DebugBreak(); } while (zero)
#define TODO_W(x, ...) do { static bool beenHere = false; if (!beenHere) { Log(FLPREFIX "TODO: " x "\n", basename(__FILE__), __LINE__, __VA_ARGS__); beenHere = true; } } while (zero)
#define LOG_WARN(f, ...) do { Log(FLPREFIX "WARN: " f "\n", basename(__FILE__), __LINE__, __VA_ARGS__); } while(zero)
#define ERR(x, ...) do { Log(FLPREFIX "ERR : " x "\nFATAL\n", basename(__FILE__), __LINE__, __VA_ARGS__); DebugBreak(); } while(zero)
#else
#define TODO(x, ...) do { Log(FLPREFIX "TODO: " x "\nFATAL\n", basename(__FILE__), __LINE__, ##__VA_ARGS__); DebugBreak(); } while (zero)
#define TODO_W(x, ...) do { static bool beenHere = false; if (!beenHere) { Log(FLPREFIX "TODO: " x "\n", basename(__FILE__), __LINE__, ##__VA_ARGS__); beenHere = true; } } while (zero)
#define LOG_WARN(f, ...) do { Log(FLPREFIX "WARN: " f "\n", basename(__FILE__), __LINE__, ##__VA_ARGS__); } while(zero)
#define ERR(x, ...) do { Log(FLPREFIX "ERR : " x "\nFATAL\n", basename(__FILE__), __LINE__, ##__VA_ARGS__); DebugBreak(); } while(zero)
#endif

#endif

