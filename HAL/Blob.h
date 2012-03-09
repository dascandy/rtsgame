#ifndef BLOB_H
#define BLOB_H

#include <string>
#include <stddef.h>
#include "Dll.h"

class HALSPEC Blob {
public:
	// Note: this one takes/steals your buffer (and deletes it with delete[])
	Blob(size_t size = 0, char *content = 0);
	// null-terminated input, to be copied
	Blob(const char *content);
	Blob(const std::string &str);
	Blob(const Blob &rhs); //Blob &&rhs);
	Blob &operator=(const Blob &rhs); //Blob &&rhs);
	~Blob();
	char *data;
	size_t size;
};

#endif

