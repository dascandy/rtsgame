#ifndef MUSICTRACK_H
#define MUSICTRACK_H

#include <string>

class MusicTrack
{
public:
	MusicTrack();
	~MusicTrack();
	void mix(signed short *data, int samples);
	void SetSource(const std::string &name);
private:
	struct Data;
	Data *_imp;

	signed short *ownData;
	enum {
		Stopped,
		Playing,
	} state;

	std::string name;

	static size_t read(void *ptr, size_t size, size_t nmemb, void *datasource);
#ifdef _MSC_VER
	static int seek(void *datasource, long long offset, int whence);
#else
	static int seek(void *datasource, long int offset, int whence);
#endif
	static int close(void *datasource);
	static long tell(void *datasource);
};

#endif

