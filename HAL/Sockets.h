#ifndef SOCKETS_H
#define SOCKETS_H

#include "Dll.h"

class HALSPEC Sockets {
	static void Init();
	static void Fini();
	friend class ServerSocket;
	friend class ClientSocket;
};

#endif

