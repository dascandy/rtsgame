#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <map>
#include "Dll.h"

class ClientSocket;
class Thread;
class HttpRequest;
class HttpReply;

class WEBSERVERSPEC Client {
private:
	ClientSocket *cs;
	HttpRequest *req;
	HttpReply *reply;
public:
	Client(ClientSocket *cs);
	~Client();
	void handle();
	void enterBlocking();
	void enterHandle();
};

#endif

