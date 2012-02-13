#ifndef STREAM_H
#define STREAM_H

#include <fstream>
#include <string>

class ResourceFile;

class stream
{
public:
	enum seekdir
	{
		beg,
		cur,
		end
	};
	stream(const stream &stream);
	~stream();
	int read(void *buffer, int count);
	int write(const void *buffer, int count);
	std::string readline();
	void writeline(std::string line);
	bool eof();
	int seek(int offset, seekdir seekdirection);
	int tell();
	int length;
private:
	friend class ResourceFile;
	stream(std::string filename, bool writable = false);
	stream &operator=(const stream &);
	int startOffset;
	int curOffset;
	std::fstream instream;
	std::string filename;
	bool writable;
};

stream &operator>>(stream &str, unsigned int &val);
stream &operator>>(stream &str, float &val);
stream &operator>>(stream &str, double &val);
stream &operator>>(stream &str, std::string &val);
stream &operator>>(stream &str, int &val);
stream &operator>>(stream &str, bool &val);

#endif

