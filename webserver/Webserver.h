#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Thread.h"

#include <string>
#include <vector>
#include <utility>
#include <map>

class HttpRequest {
public:
	std::string method, url, httpversion;
	std::map<std::string, std::string> attributes;
	std::map<std::string, std::string> arguments;
	int fd;
	Thread *t;
	HttpRequest();
	void handle();
	void readFrom(int fd);
};

class HttpReply {
public:
	HttpReply(int code, std::string text, const char *contentType, const char *content);
	HttpReply();
	void writeTo(int fd);
	int statusCode;
	std::string statusText;
	const char *contentType;
	const char *content;
	int contentLength;
};

HttpReply defaultReply(int statuscode);
std::string getMimetype(std::string ext);

class Webserver {
public:
	class Callback {
	public:
		virtual void handle(HttpRequest &req) = 0;
	};
private:
	std::vector<std::pair<std::string, Callback *> > urls;
	int listenfd;
	bool stopped;
	Thread t;
	void handleDefault(HttpRequest &req);
	void run();
	static void stop();
	Webserver(int port);
public:
	void handle(HttpRequest &req);
	static Webserver &Instance() { static Webserver webserver(1080); return webserver; }
	void registerUrl(std::string match, Callback *callback);
};

#endif


