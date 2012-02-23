#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include "Dll.h"

class HALSPEC Entry {
public:
	static Entry *Create(std::string path);
	virtual bool isDirectory() = 0;
	virtual std::string getFullName() = 0;
	virtual size_t getSize() = 0;
};

#endif

