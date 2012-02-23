#ifndef BLOB_H
#define BLOB_H

class blob {
public:
	blob(int size, void *buffer = 0) : size(size), buffer(buffer) {
		if (!buffer) {
			this->buffer = new char[size];
		}
	}
	int size;
	void *buffer;
};

#endif


