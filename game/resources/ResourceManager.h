#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>

class ResourceManager {
public:
	static ResourceManager &Instance() { static ResourceManager rm; return rm; }
	void setRootDir(std::string rootDir) { 
	}
};

#endif


