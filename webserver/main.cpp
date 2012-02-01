#include "Webserver.h"
#include "Html.h"
#include <stdio.h>

class TestCallback : public Webserver::Callback {
public:
	void handle(HttpRequest &hr) {
		HtmlPage page;
		page.addDiv(new Div(50, 50, 75, 75, "file.gif"));
		page.addDiv(new Div(150, 50, 75, 75, "folder.jpg"));
		page.addDiv(new Div(50, 150, 75, 75, "link.png"));
		page.addDiv(new Div(150, 150, 75, 75, "unknown.png"));
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
	Webserver::Instance().registerUrl("/test", &tc);
	for (;;) {}
}


