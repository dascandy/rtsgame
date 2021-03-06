#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Thread.h"
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <deque>
#include "ServerSocket.h"
#include "Dll.h"

class HttpRequest;
class HttpReply;

class WEBSERVERSPEC Webserver {
public:
	class WEBSERVERSPEC Callback {
	public:
		virtual HttpReply handle(HttpRequest &req) = 0;
	};
private:
	std::vector<std::pair<std::string, Callback *> > urls;
	ServerSocket server;
	bool stopped;
	Thread t;
	void run();
	static void stop();
	Webserver(unsigned short port);
	static bool matches(const std::string &a, const std::string &b);
	std::deque<std::pair<HttpRequest *, ClientSocket *> > queuedRequests;
public:
	HttpReply handle(HttpRequest &req);
	void Handle(HttpRequest *req, ClientSocket *cs);
	static Webserver &Instance() { static Webserver webserver(1080); return webserver; }
	void registerUrl(std::string match, Callback *callback);
};

#include "HttpRequest.h"
#include "HttpReply.h"
#include "Html.h"

#endif


