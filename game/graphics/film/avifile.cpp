#include "AviFile.h"
#include "ResourceFile.h"
#include "debug.h"

#define BW10 0x62773130
#define BD00 0x62643030
#define CD00 0x63643030
#define RRV1 0x31767272
#define JYV1 0x3176796A
#define RRV2 0x32767272

static struct {
	unsigned int pad1[12];
	unsigned int framecount;
	unsigned int pad2[3];
	unsigned int w, h;
	unsigned int pad3[10];
	unsigned int fourcc;
} aviheader;

AviFile::AviFile(const std::string &name)
: in(ResourceFile::open(name, ResourceFile::Movie))
{
	//TODO: make AVI header parsing
	in.read(&aviheader, sizeof(aviheader));
	w = aviheader.w;
	h = aviheader.h;
	frameCount = aviheader.framecount;
	frameIndex = 0;
	switch(aviheader.fourcc)
	{
	case RRV1:
	case JYV1:
		chunks = 5;
		break;
	case RRV2:
		chunks = 15;
		break;
	default:
		TODO("Make this type of video work too");
		break;
	}
	chunksize = w * h / chunks;
}

void *AviFile::Frame(msec_t delta)
{
	if (frameIndex >= frameCount)
		return NULL;

	do
	{
		struct {
			unsigned int chunkType;
			unsigned int chunkSize;
		} header;
		in.read(&header, sizeof(header));
		void *data = malloc(header.chunkSize);
		in.read(data, header.chunkSize);
		switch(header.chunkType)
		{
		case BW10:
			ParseAudioPacket(header.chunkSize, (signed char *)data);
			break;
		case CD00:
		case BD00:
			ParseVideoPacket(header.chunkSize, (unsigned char *)data);
			return videoBuffer;
		default:
			// stream error!
			return NULL;
		}
	}
	while (true);
}

void AviFile::mix(signed short *target, int sampleCount)
{
	if (audioBuffer.size() == 0)
		return;

	AudioFrame *headFrame = audioBuffer.front();

	for (int i = 0; i < sampleCount*2; i+=2)
	{
		target[i] += (signed short)headFrame->buffer[audioSampleIndex/4] << 4;
		target[i+1] += (signed short)headFrame->buffer[audioSampleIndex/4] << 4;
		audioSampleIndex++;
		if (audioSampleIndex == headFrame->samples * 4)
		{
			delete [] headFrame->buffer;
			delete headFrame;
			audioBuffer.pop_front();
			if (audioBuffer.size() == 0)
				return;

			headFrame = audioBuffer.front();
			audioSampleIndex = 0;
		}
	}
}

void AviFile::ParseAudioPacket(int chunkLength, signed char *chunkData)
{
	audioBuffer.push_back(new AudioFrame(chunkLength, (signed char *)chunkData));
}

void AviFile::ParseVideoPacket(int, unsigned char *chunkData)
{
	int *offses = (int *)chunkData;
	for (int i = 0; i < chunks; i++)
	{
		parse_video_subchunk(chunkData + offses[i], videoBuffer + i * chunksize);
	}

	delete [] chunkData;
}

int AviFile::get_nibble(const unsigned char *chunk, size_t &nibbleoffs)
{
	if (nibbleoffs & 1)
		return chunk[nibbleoffs++ / 2] & 0xF;

	return (chunk[nibbleoffs++ / 2] >> 4) & 0xF;
}

void AviFile::parse_video_subchunk(const unsigned char *chunk, unsigned char *vbuffer)
{
	bool exitReceived = false;
	unsigned int commandsize;
	memcpy(&commandsize, chunk, 4);
	const unsigned char *commandptr = chunk + 4;
	const unsigned char *dataptr = chunk + 4 + commandsize;
	size_t nibbleoffs = 0;

	while (nibbleoffs / 2 < (int)commandsize &&
		!exitReceived)
	{
		unsigned int num = 0;
		int nibble = get_nibble(commandptr, nibbleoffs);
		switch(nibble)
		{
		case 0:
			if (nibbleoffs >= commandsize * 2) 
				break;

			switch(get_nibble(commandptr, nibbleoffs))
			{
			case 0:
				exitReceived = true;
				break;
			case 5:
				for (int i = 0; i < 4; i++)
				{
					num = (num << 4) | get_nibble(commandptr, nibbleoffs);
				}
				memcpy(vbuffer, dataptr, num);
				vbuffer += num;
				dataptr += num;
				break;
			default:
				return;
			}
			break;
		default:
			return;
		}
	}
}

