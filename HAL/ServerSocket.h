#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include "ClientSocket.h"
#include "Dll.h"

class HALSPEC ServerSocket {
public:
	ServerSocket(unsigned short port);
	~ServerSocket();
	ClientSocket *Accept(unsigned short &portno, std::string &ipaddr);
private:
	SOCK_TYPE sock;
};

#endif

