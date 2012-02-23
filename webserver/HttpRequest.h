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
	ClientSocket *cs;
	Thread *t;
	HttpRequest();
	void handle();
	void readFrom();
};

#endif

