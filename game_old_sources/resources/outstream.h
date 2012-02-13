#ifndef OUTSTREAM_H
#define OUTSTREAM_H

#include <fstream>
#include <string>

class ResourceFile;

class outstream
{
public:
	enum seekdir
	{
		beg,
		cur,
		end
	};
	outstream(const outstream &outstream);
	~outstream();
	int write(const void *buffer, int count);
	outstream(std::string filename);
private:
	outstream &operator=(const outstream &);
	std::ofstream outputstream;
	std::string filename;
};

outstream &operator<<(outstream &str, unsigned int val);
outstream &operator<<(outstream &str, float val);
outstream &operator<<(outstream &str, double val);
outstream &operator<<(outstream &str, const std::string &val);
outstream &operator<<(outstream &str, const char *val);
outstream &operator<<(outstream &str, int val);
outstream &operator<<(outstream &str, bool val);

#endif

