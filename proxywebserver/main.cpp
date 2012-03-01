#include "ClientSocket.h"
#include "Webserver.h"
#include <stdio.h>

class Handler : public Webserver::Callback {
	HttpReply handle(HttpRequest &request) {
		printf("%s requested through proxy\n", request.url.c_str());
		return HttpReply::defaultReply(404);
	}
};

static Handler &getHandler() { static Handler handler; return handler; }

int main() {
	Webserver::Instance().registerUrl("/", &getHandler());
	while(1) {}
}


