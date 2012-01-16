#ifndef AVIFILE_H
#define AVIFILE_H

#include "Movie.h"
#include <string>
#include "stream.h"
#include <list>

class AviFile : public Movie
{
public:
	AviFile(const std::string &name);
	void *Frame(msec_t delta);
	void mix(signed short *target, int sampleCount);
	const std::string &Subtitle();
private:
	stream in;
	int frameIndex, frameCount;
	int chunks;
	int chunksize;
	int w, h;

	char palette[1024];
	unsigned char *videoBuffer;

	struct AudioFrame
	{
		AudioFrame(int samples, signed char *buffer)
			: samples(samples)
			, buffer(buffer)
		{}
		int samples;
		signed char *buffer;
	};
	std::list<AudioFrame *> audioBuffer;
	int audioSampleIndex;

	// Private AVI parsing functions
	void ParseAudioPacket(int chunkLength, signed char *chunkData);
	void ParseVideoPacket(int chunkLength, unsigned char *chunkData);

	int get_nibble(const unsigned char *chunk, size_t &nibbleoffs);
	void parse_video_subchunk(const unsigned char *chunk, unsigned char *vbuffer);
};

#endif

