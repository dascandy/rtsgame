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

	static size_t MusicTrack::read(void *ptr, size_t size, size_t nmemb, void *datasource);
	static int MusicTrack::seek(void *datasource, long long offset, int whence);
	static int MusicTrack::close(void *datasource);
	static long MusicTrack::tell(void *datasource);
};

#endif

