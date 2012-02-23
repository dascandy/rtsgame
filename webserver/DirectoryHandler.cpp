#include "DirectoryHandler.h"
#include "Entry.h"
#include "File.h"
#include "Directory.h"

WEBSERVERSPEC void DirectoryHandler::handle(HttpRequest &req) {
	std::string pathname = "htdocs" + req.url;
	Entry *ent = Entry::Create(pathname);
	if (ent->isDirectory()) {
		writeDirReply(req.cs, (Directory *)ent);
	} else {
		File *f = (File *)ent;
		HttpReply(200, "Ok", HttpReply::getMimetype(f->getExtension()), f->read()).writeTo(req.cs);
	}
}

WEBSERVERSPEC void DirectoryHandler::writeDirReply(ClientSocket *cs, Directory *dir) {
	std::string acquiredData = "<html><head><title>Dir list of " + dir->getFullName() + "</title></head><body><table>";
	std::vector<Entry *> entries = dir->getEntries();
	for (std::vector<Entry *>::iterator it = entries.begin(); it != entries.end(); ++it) {
		char buffer[2048];
		sprintf(buffer, "<tr><td><img width=\"20\" height=\"20\" src=\"%s\"></td><td></td><td><a href=\"%s\">%s</a></td></tr>\n", 
			(*it)->isDirectory() ? "/folder.jpg" : "/file.gif", (*it)->getSize(), (*it)->getFullName(), (*it)->getFullName());
		acquiredData += buffer;
	}
	acquiredData += "</table></body></html>";
	HttpReply(200, "Ok", "text/html", acquiredData.c_str()).writeTo(cs);
}
