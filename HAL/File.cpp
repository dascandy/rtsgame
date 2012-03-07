#include "File.h"
#include <stdio.h>
#include "Blob.h"

#ifdef _MSC_VER
#define SEP "\\"
#else
#define SEP "/"
#endif

HALSPEC File::File(std::string path, size_t size) 
: size(size)
{
	parent = path.substr(0, path.find_last_of(SEP));
	path = path.substr(path.find_last_of(SEP)+1);
	name = path.substr(0, path.find_last_of('.'));
	ext = path.substr(path.find_last_of('.')+1);
}

HALSPEC std::string File::getFullName() {
	return parent + SEP + name + "." + ext;
}

HALSPEC Blob File::read() {
	Blob data(size);
	FILE *f = fopen(getFullName().c_str(), "rb");
	fread((void *)data.data, size, 1, f);
	fclose(f);
	return data;
}

File *File::Create(std::string fullPath, Blob newContent) {
	FILE *f = fopen(fullPath.c_str(), "wb");
	fwrite(newContent.data, newContent.size, 1, f);
	fclose(f);
	return new File(fullPath, newContent.size);
}

