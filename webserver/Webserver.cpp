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
		HttpRequest *request = new HttpRequest();
		request->cs = cs;
		request->t = new Thread();
		request->t->start(request, &HttpRequest::handle);
	}
}

bool Webserver::matches(const std::string &a, const std::string &b) {
	if (b.find(a) == 0) return true;
	return false;
}

void Webserver::handle(HttpRequest &hr) {
	std::vector<std::pair<std::string, Webserver::Callback *> >::iterator it = urls.begin();
	for (; it != urls.end(); ++it) {
		if (matches(it->first, hr.url)) {
			try {
				it->second->handle(hr);
			} catch (std::exception &e) {
				const char *t = e.what();
				Blob data(strlen(t)+98);
				sprintf(data.data, "<html><head><title>Exception occurred while processing request</title></head><body>%s</body></html>", t);
				HttpReply::defaultReply(500).setContent(data).writeTo(hr.cs);
			} catch (...) {
				HttpReply::defaultReply(500).writeTo(hr.cs);
			}
			return;
		}
	}
	HttpReply::defaultReply(404).writeTo(hr.cs);
}

void Webserver::registerUrl(std::string match, Webserver::Callback *callback) {
	urls.push_back(std::make_pair(match, callback));
}

