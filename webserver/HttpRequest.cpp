#include "HttpRequest.h"
#include "Thread.h"
#include "ClientSocket.h"
#include "Webserver.h"

HttpRequest::HttpRequest() {}

static std::string readLineFrom(ClientSocket *cs) {
	char buffer[1024];
	int idx = 0;
	do {
		cs->read(&buffer[idx++], 1);
	} while (idx < 2 || buffer[idx-2] != '\r' && buffer[idx-1] != '\n');
	return std::string(buffer, 0, idx-2);
}

void HttpRequest::readFrom() {
	std::string requestLine = readLineFrom(cs);
	int firstSpace = requestLine.find(" ", 0);
	int secondSpace = requestLine.find(" ", firstSpace+1);
	method = requestLine.substr(0, firstSpace);
	url = requestLine.substr(firstSpace+1, secondSpace - firstSpace - 1);
	httpversion = requestLine.substr(secondSpace+1);
	std::string argLine = readLineFrom(cs);
	while (argLine.size()) {
		std::string name, arg;
		int separator = argLine.find(": ", 0);
		name = argLine.substr(0, separator);
		arg = argLine.substr(separator+2);
		attributes.insert(std::make_pair(name, arg));
	
		argLine = readLineFrom(cs);
	}

	if (url.find_first_of('?') != std::string::npos) {
		std::string args = url.substr(url.find_first_of('?')+1);
		url = url.substr(0, url.find_first_of('?'));
		while (args.size() > 0) {
			int nextAmp = args.find_first_of('&');
			std::string nextArg;
			if (nextAmp == std::string::npos) {
				nextArg = args;
				args = "";
			} else {
				nextArg = args.substr(0, nextAmp);
				args = args.substr(nextAmp+1);
			}
			std::string val = "true";
			int eq = nextArg.find_first_of('=');
			if (eq != std::string::npos) {
				val = nextArg.substr(eq+1);
				nextArg = nextArg.substr(0, eq);
			}
			arguments.insert(std::make_pair(nextArg, val));
		}
	}

	if (url[url.size()-1] == '/') url = url.substr(0, url.size()-1);
}

void HttpRequest::handle() {
	readFrom();
	Webserver::Instance().handle(*this);
	// TODO: allow reuse here with keepalive
	delete cs;
}