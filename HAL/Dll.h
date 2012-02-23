#ifndef DLL_H
#define DLL_H

#ifdef _MSC_VER
#ifdef HAL_COMPILE
#define HALSPEC __declspec(dllexport)
#else
#define HALSPEC __declspec(dllimport)
#endif
#ifdef WEBSERVER_COMPILE
#define WEBSERVERSPEC __declspec(dllexport)
#else
#define WEBSERVERSPEC __declspec(dllimport)
#endif

#else
#define HALSPEC
#define WEBSERVERSPEC
#endif

#endif

