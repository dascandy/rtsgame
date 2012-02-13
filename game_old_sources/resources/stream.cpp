#include "stream.h"
#include <iostream>
#include <fstream>

stream::stream(std::string filename, bool writable)
: startOffset(0)
, curOffset(0)
, length(0)
, instream(filename.c_str(), std::fstream::binary | (writable ? std::fstream::out | std::fstream::trunc : std::fstream::in))
, filename(filename)
, writable(writable)
{
	if (instream.fail())
		throw std::exception();

	instream.exceptions(std::ios::failbit | std::ios::eofbit | std::ios::badbit);

	if (!writable)
	{
		instream.seekg(0, std::ios_base::end);
		length = instream.tellg();
		instream.seekg(0, std::ios_base::beg);
	}
}

stream::stream(const stream &otherstream)
: startOffset(otherstream.startOffset)
, curOffset(otherstream.curOffset)
, length(otherstream.length)
, instream(otherstream.filename.c_str(), std::ifstream::binary | (otherstream.writable ? std::ifstream::out | std::fstream::trunc : std::fstream::in))
, filename(otherstream.filename) 
, writable(otherstream.writable)
{
	if (instream.fail())
		throw std::exception();

	instream.exceptions(std::ios::failbit | std::ios::eofbit | std::ios::badbit);
}

stream::~stream() 
{
}

bool stream::eof()
{
	return (curOffset == length);
}

int stream::read(void *bufp, int count)
{
	if (curOffset + count > length)
		count = length - curOffset;

	instream.seekg(startOffset, std::ios_base::beg);
	instream.seekg(curOffset, std::ios_base::cur);
	instream.read((char *)bufp, count);
	curOffset += count;
	return count;
}

int stream::write(const void *bufp, int count)
{
	instream.seekg(startOffset, std::ios_base::beg);
	instream.seekg(curOffset, std::ios_base::cur);
	instream.write((char *)bufp, count);
	curOffset += count;
	return count;
}

std::string stream::readline()
{
	char cbuf = 'x';
	std::string str;
	while (curOffset < length)
	{
		instream.seekg(startOffset + curOffset, std::ios_base::beg);
		instream.read(&cbuf, 1);
		curOffset++;
		if (cbuf == 10)
			break;
		if (cbuf >= 32) 
			str.push_back(cbuf);
	}

	return str;
}

void stream::writeline(std::string line)
{
	instream.seekg(startOffset + curOffset, std::ios_base::beg);
	instream.write(line.c_str(), line.size());
	curOffset += line.size();
	if (curOffset > length) length = curOffset;
}

int stream::seek(int offset, seekdir seekdirection)
{
	if (seekdirection == beg)
		curOffset = 0;
	else if (seekdirection == end)
		curOffset = length;

	if (curOffset + offset > length)
	{
		curOffset = length;
	}
	else if (curOffset + offset < 0)
	{
		curOffset = 0;
	}
	else
	{
		curOffset += offset;
	}

	return curOffset;
}

int stream::tell()
{
	return curOffset;
}

stream &operator>>(stream &str, unsigned int &val)
{
	str.read(&val, sizeof(unsigned int));
	return str;
}

stream &operator>>(stream &str, float &val)
{
	str.read(&val, sizeof(float));
	return str;
}

stream &operator>>(stream &str, double &val)
{
	str.read(&val, sizeof(double));
	return str;
}

stream &operator>>(stream &str, std::string &val)
{
	unsigned int length = 0;
	unsigned char lengthbyte;
	int lengthidx;
	str.read(&lengthbyte, 1);
	for (lengthidx = 0; lengthbyte & 0x80; lengthidx++)
	{
		length |= (lengthbyte & 0x7F) << (lengthidx * 7);
		str.read(&lengthbyte, 1);
	}
	length |= (lengthbyte & 0x7F) << (lengthidx * 7);
	val.resize(length);
	str.read(&val[0], length);
	return str;
}

stream &operator>>(stream &str, int &val)
{
	str.read(&val, sizeof(int));
	return str;
}

stream &operator>>(stream &str, bool &val)
{
	int value;
	str.read(&value, 1);
	val = (value == 1);
	return str;
}

