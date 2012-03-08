#include "Webserver.h"
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "Blob.h"
#include "HttpRequest.h"
#include "HttpReply.h"
#include "Client.h"

Webserver::Webserver(int port) 
: server(port)
, stopped(false)
{
	t.start(this, &Webserver::run);
	atexit(&stop);
}

void Webserver::stop() {
	Webserver::Instance().t.stop();
}

void Webserver::run() {
	while (!stopped) {
		std::string addr;
		unsigned short port;
		ClientSocket *cs = server.Accept(port, addr);
		printf("Getting new client at %s:%d %p\n", addr.c_str(), port, cs);
		if (cs) 
			Client *c = new Client(cs);
	}
	printf("Webserver stopped");
}

bool Webserver::matches(const std::string &a, const std::string &b) {
	if (b.find(a) == 0) return true;
	return false;
}

HttpReply Webserver::handle(HttpRequest &hr) {
	std::vector<std::pair<std::string, Webserver::Callback *> >::iterator it = urls.begin();
	for (; it != urls.end(); ++it) {
		if (matches(it->first, hr.url)) {
			try {
				return it->second->handle(hr);
			} catch (std::exception &e) {
				const char *t = e.what();
				Blob data(strlen(t)+98);
				sprintf(data.data, "<html><head><title>Exception occurred while processing request</title></head><body>%s</body></html>", t);
				return HttpReply::defaultReply(500).setContent(data);
			} catch (...) {
				return HttpReply::defaultReply(500);
			}
		}
	}
	return HttpReply::defaultReply(404);
}

void Webserver::DoPoll() {
	Webserver::Instance().Poll();
}

void Webserver::Poll() {
	while (queuedRequests.size()) {
		std::pair<HttpRequest *, ClientSocket *> p = queuedRequests.front();
		queuedRequests.pop_front();
		handle(*p.first).writeTo(p.second);
	}
}

void Webserver::Queue(HttpRequest *req, ClientSocket *cs) {
	queuedRequests.push_back(std::make_pair(req, cs));
}

void Webserver::registerUrl(std::string match, Webserver::Callback *callback) {
	urls.push_back(std::make_pair(match, callback));
}

