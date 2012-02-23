#include "Sockets.h"

#ifdef _MSC_VER
#include "windows.h"
#include "winsock.h"

HALSPEC void Sockets::Init() {
	WSADATA data;
	WSAStartup(0x0201, &data);
	atexit(&Sockets::Fini);
}

HALSPEC void Sockets::Fini() {
	WSACleanup();
}
#else
void Sockets::Init() {}
void Sockets::Fini() {}
#endif
