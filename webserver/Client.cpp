#include "Client.h"
#include "Thread.h"
#include "ClientSocket.h"
#include "HttpRequest.h"
#include "Webserver.h"

Client::Client(ClientSocket *cs) 
: cs(cs)
{
	t = new Thread();
	t->start(this, &Client::handle);
}

Client::~Client() {
	delete cs;
	delete t;
}

void Client::handle() {
	bool keepalive = false;
	do {
		HttpRequest req(cs);
		if (req.arguments["Connection"] == "Keep-Alive") keepalive = true;
		Webserver::Instance().handle(req).writeTo(cs);
	} while (keepalive);
	delete this;
}
