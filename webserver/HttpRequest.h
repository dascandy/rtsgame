#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <map>
#include "Dll.h"

class ClientSocket;
class Thread;

class WEBSERVERSPEC HttpRequest {
public:
	std::string method, url, httpversion;
	std::map<std::string, std::string> attributes;
	std::map<std::string, std::string> arguments;
	HttpRequest(ClientSocket *cs);
	HttpRequest(std::string method, std::string url, std::string httpversion);
	void readFrom(ClientSocket *cs);
	void writeTo(ClientSocket *cs);
};

#endif

