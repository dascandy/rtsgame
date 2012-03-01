#include "ClientSocket.h"
#include "Webserver.h"

class Handler : public Webserver::Callback {
	HttpReply handle(HttpRequest &request) {
		
	}
};

static Handler &getHandler() { static Handler handler; return handler; }

int main() {
	Webserver::Instance().registerUrl("/", getHandler());
	while(1) {}
}


