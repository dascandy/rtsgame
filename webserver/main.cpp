#include "Webserver.h"
#include "Html.h"
#include <stdio.h>

class TestCallback : public Webserver::Callback {
public:
	void handle(HttpRequest &hr) {
		HtmlPage page;
		page.addDiv(new ImageDiv(0, 0, 1200, 600, "/file.gif"));
		HttpReply reply(defaultReply(200));
		std::string pageContent = page.toString();
		reply.contentType = "text/html";
		reply.content = pageContent.c_str();
		reply.contentLength = pageContent.size();
		reply.writeTo(hr.fd);
	}
};

class GlslCallback : public Webserver::Callback {
public:
	void handle(HttpRequest &hr) {
		HtmlPage page;
		page.addDiv(new DocumentDiv(0, 0, 1200, 600, hr.url));
		HttpReply reply(defaultReply(200));
		std::string pageContent = page.toString();
		reply.contentType = "text/html";
		reply.content = pageContent.c_str();
		reply.contentLength = pageContent.size();
		reply.writeTo(hr.fd);
	}
};

int main() {
	Webserver::Instance();
	TestCallback tc;
	GlslCallback gc;
	Webserver::Instance().registerUrl("/test", &tc);
	Webserver::Instance().registerUrl("/glsl/", &gc);
	for (;;) {}
}


