#include "outstream.h"
#include <iostream>
#include <fstream>

outstream::outstream(std::string filename)
: outputstream(filename.c_str(), std::ofstream::binary | std::ofstream::out | std::ofstream::trunc),
  filename(filename) 
{
	outputstream.exceptions(std::ios::failbit | std::ios::eofbit | std::ios::badbit);
}

outstream::outstream(const outstream &otheroutstream)
: outputstream(otheroutstream.filename.c_str(), std::ofstream::binary | std::ofstream::out | std::ofstream::trunc),
  filename(otheroutstream.filename) 
{
	outputstream.exceptions(std::ios::failbit | std::ios::eofbit | std::ios::badbit);
}

outstream::~outstream()
{
}

int outstream::write(const void *bufp, int count) 
{
	outputstream.write((const char *)bufp, count);
	return count;
}

outstream &operator<<(outstream &str, unsigned int val)
{
	str.write(&val, sizeof(unsigned int));
	return str;
}

outstream &operator<<(outstream &str, float val)
{
	str.write(&val, sizeof(float));
	return str;
}

outstream &operator<<(outstream &str, double val)
{
	str.write(&val, sizeof(double));
	return str;
}

outstream &operator<<(outstream &str, const char *val)
{
	str << std::string(val);
	return str;
}

outstream &operator<<(outstream &str, const std::string &val)
{
	unsigned int length = val.size();
	while (length != 0)
	{
		unsigned char value = length & 0x7F;
		length >>= 7;
		if (length != 0) value |= 0x80;
		str.write(&value, 1);
	}

	length = val.size();
	str.write(&val[0], length);
	return str;
}

outstream &operator<<(outstream &str, int val)
{
	str.write(&val, sizeof(int));
	return str;
}

outstream &operator<<(outstream &str, bool val)
{
	int value = (val ? 1 : 0);
	str.write(&value, 1);
	return str;
}

