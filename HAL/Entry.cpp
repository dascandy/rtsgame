#include "Entry.h"
#include "File.h"
#include "Directory.h"
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#define SEP '\\'
#else
#define SEP '/'
#endif

#ifdef _MSC_VER
HALSPEC Entry *Entry::Create(std::string path) {
	while (path[path.size()-1] == SEP) path = path.substr(0, path.size()-1);
	struct __stat64 data;
	int statr = _stat64(path.c_str(), &data);
	if (data.st_mode & _S_IFREG) {
		return new File(path, data.st_size);
	} else if (data.st_mode & _S_IFDIR) {
		return new Directory(path, data.st_size);
	} else {
		return 0;
	}
}
#else
Entry *Entry::Create(const char *path) {
	while (path[path.size()-1] == SEP) path = path.substr(0, path.size()-1);
	struct stat data;
	int statr = stat(filename.c_str(), &data);
	if (S_ISREG(data.st_mode)) {
		return new File(path, data.st_size);
	} else if (S_ISDIR(data.st_mode)) {
		return new Directory(path, data.st_size);
	} else {
		return 0;
	}
}
#endif

