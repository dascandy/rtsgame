#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <map>
#include "Dll.h"

class ClientSocket;
class Thread;

class WEBSERVERSPEC Client {
private:
	ClientSocket *cs;
	Thread *t;
public:
	Client(ClientSocket *cs);
	~Client();
	void handle();
};

#endif

