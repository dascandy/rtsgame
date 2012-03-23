#include "Client.h"
#include "Thread.h"
#include "ClientSocket.h"
#include "HttpRequest.h"
#include "Webserver.h"
#include "QueuedWork.h"

class QueuedClientBlocking : public Queued {
	Client *c;
public:
	QueuedClientBlocking(Client *c) : c(c) {}
	void run() {
		c->enterBlocking();
	}
};

class QueuedClientHandling : public Queued {
	Client *c;
public:
	QueuedClientHandling(Client *c) : c(c) {}
	void run() {
		c->enterHandle();
	}
};

Client::Client(ClientSocket *cs) 
: cs(cs)
, reply(0)
, req(0)
{
	QueuedWork::Queue(new QueuedClientBlocking(this), QueuedWork::Blocking);
}

Client::~Client() 
{
	delete cs;
	delete req;
	delete reply;
}

void Client::enterBlocking() 
{
	if (reply) {
		if (req->attributes["Connection"] == "Keep-Alive") {
			reply->attributes["Connection"] = "Keep-Alive";
			reply->writeTo(cs);
		} else {
			reply->writeTo(cs);
			delete this;
			return;
		}
	}
	delete req;
	delete reply;
	req = new HttpRequest(cs);
	QueuedWork::Queue(new QueuedClientHandling(this), QueuedWork::OpenGL);
}

void Client::enterHandle() {
	reply = new HttpReply(Webserver::Instance().handle(*req));
	QueuedWork::Queue(new QueuedClientBlocking(this), QueuedWork::Blocking);
}

