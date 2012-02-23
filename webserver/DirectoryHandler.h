#ifndef DIRECTORYHANDLER_H
#define DIRECTORYHANDLER_H

#include "Webserver.h"
#include "Dll.h"

class Directory;

class WEBSERVERSPEC DirectoryHandler : public Webserver::Callback {
public:
	void handle(HttpRequest &req);
	void writeDirReply(ClientSocket *cs, Directory *dir);
};

#endif

