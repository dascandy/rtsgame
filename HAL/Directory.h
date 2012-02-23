#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "Entry.h"
#include <vector>
#include "Dll.h"

class HALSPEC Directory : public Entry {
private:
	friend class Entry;
	Directory(std::string path, size_t size);
	std::string path;
	size_t size;
public:
	std::string getFullName() { return path; }
	size_t getSize() { return size; }
	bool isDirectory() { return true; }
	const char *getIconName();
	std::vector<Entry *> getEntries();
};

#endif

