#ifndef HTTPREPLY_H
#define HTTPREPLY_H

#include <string>
#include "Blob.h"
#include "Dll.h"

class ClientSocket;

class WEBSERVERSPEC HttpReply {
public:
	HttpReply(int code, std::string text, const char *contentType, Blob content = Blob());
	HttpReply();
	void writeTo(ClientSocket *cs);
	static const char *textFor(int statuscode);
	static HttpReply defaultReply(int statuscode);
	static const char *getMimetype(std::string ext);
	HttpReply &setContent(Blob content);
	int statusCode;
	std::string statusText;
	const char *contentType;
	Blob content;
};

#endif

