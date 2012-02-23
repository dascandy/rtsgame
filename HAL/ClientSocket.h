#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <string>

#ifdef _MSC_VER
typedef unsigned int SOCK_TYPE;
#else
typedef int SOCK_TYPE;
#endif

#include "Dll.h"

class HALSPEC ClientSocket {
private:
	SOCK_TYPE fd;
	ClientSocket(SOCK_TYPE fd);
	friend class ServerSocket;
public:
	int read(char *buffer, int max);
	int write(const char *buffer, int count);
	ClientSocket(std::string host, unsigned short port);
	~ClientSocket();
};

#endif

