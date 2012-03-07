#ifndef FILE_H
#define FILE_H

#include <string>
#include "Entry.h"
class Blob;

#include "Dll.h"

class HALSPEC File : public Entry {
private:
	size_t size;
	std::string parent;
	std::string name;
	std::string ext;
	File(std::string fullPath, size_t size);
	friend class Entry;
public:
	static File *Create(std::string fullPath, Blob newContent);
	std::string getFullName();
	size_t getSize() { return size; }
	const std::string &getExtension() { return ext; }
	bool isDirectory() { return false; }
	Blob read();
};

#endif

