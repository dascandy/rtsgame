#include "Directory.h"

#ifdef _MSC_VER
#define SEP "\\"
#else
#define SEP "/"
#endif

HALSPEC Directory::Directory(std::string path, size_t size) 
: path(path)
, size(size)
{}

#ifdef _MSC_VER
#include <windows.h>
HALSPEC std::vector<Entry *> Directory::getEntries() {
	std::vector<Entry *> entries;
	WIN32_FIND_DATA data;
	HANDLE hdl = FindFirstFile(path.c_str(), &data);
	do {
		std::string cPath = path + SEP + data.cFileName;
		entries.push_back(Entry::Create(cPath.c_str()));
	} while (FindNextFile(hdl, &data));
	FindClose(hdl);
	return entries;
}
#else
std::vector<Entry *> Directory::getEntries() {
	std::vector<Entry *> entries;
	DIR *dir = opendir(filename.c_str());
	struct dirent *ent;
	while ((ent = readdir(dir))) {
		std::string cPath = path + SEP + ent->d_name;
		entries.push_back(Entry::Create(cPath.c_str()));
	}
	closedir(dir);
	return entries;
}
#endif

