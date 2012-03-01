#include "Blob.h"
#include <string.h>

HALSPEC Blob::Blob(const char *content) {
	size = strlen(content);
	data = new char[size];
	memcpy(data, content, size);
}

HALSPEC Blob::Blob(size_t size, char *content) 
: data(content)
, size(size) {
	if (!data) {
		data = new char[size];
	}
}

HALSPEC Blob::Blob(const Blob &rhs) {
	Blob &b = const_cast<Blob &>(rhs);
	delete [] data;
	data = b.data;
	size = b.size;
	b.data = 0;
	b.size = 0;
}

HALSPEC Blob &Blob::operator=(const Blob &rhs) {
	Blob &b = const_cast<Blob &>(rhs);
	delete [] data;
	data = b.data;
	size = b.size;
	b.data = 0;
	b.size = 0;
	return *this;
}

HALSPEC Blob::~Blob() {
	delete [] data;
}

