#include "DirectoryHandler.h"
#include "Entry.h"
#include "File.h"
#include "Directory.h"
#include <stdio.h>

WEBSERVERSPEC HttpReply DirectoryHandler::handle(HttpRequest &req) {
	std::string pathname = "htdocs" + req.url;
	Entry *ent = Entry::Create(pathname);
	if (ent->isDirectory()) {
		return writeDirReply((Directory *)ent);
	} else {
		File *f = (File *)ent;
		return HttpReply(200, "Ok", HttpReply::getMimetype(f->getExtension()), f->read());
	}
}

WEBSERVERSPEC HttpReply DirectoryHandler::writeDirReply(Directory *dir) {
	std::string acquiredData = "<html><head><title>Dir list of " + dir->getFullName() + "</title></head><body><table>";
	std::vector<Entry *> entries = dir->getEntries();
	for (std::vector<Entry *>::iterator it = entries.begin(); it != entries.end(); ++it) {
		char buffer[2048];
		sprintf(buffer, "<tr><td><img width=\"20\" height=\"20\" src=\"%s\"></td><td>%u</td><td><a href=\"%s\">%s</a></td></tr>\n", 
			(*it)->isDirectory() ? "/folder.jpg" : "/file.gif", (unsigned int)(*it)->getSize(), (*it)->getFullName().c_str(), (*it)->getFullName().c_str());
		acquiredData += buffer;
	}
	acquiredData += "</table></body></html>";
	return HttpReply(200, "Ok", "text/html", acquiredData.c_str());
}
