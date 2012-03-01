#ifndef DIRECTORYHANDLER_H
#define DIRECTORYHANDLER_H

#include "Webserver.h"
#include "Dll.h"

class Directory;

class WEBSERVERSPEC DirectoryHandler : public Webserver::Callback {
public:
	HttpReply handle(HttpRequest &req);
	HttpReply writeDirReply(Directory *dir);
};

#endif

