#include "ResourceManager.h"
#include "File.h"
#include "Entry.h"

Blob ResourceManager::loadblob(const std::string &name) {
	Entry *e = Entry::Create(rootDir + "/" + name);
	if (!e) return Blob();
	if (e->isDirectory()) {
		return Blob();
	}
	return ((File *)e)->read();
}

void ResourceManager::saveblob(const std::string &name, Blob &b) {
	delete File::Create(rootDir + "/" + name, b);
}

