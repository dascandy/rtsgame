#include "FragmentDeserializer.h"
#include <SDL.h>
#include "ResourceFile.h"
#include "stream.h"

FragmentDeserializer::FragmentDeserializer(std::string name)
: name(name)
{
}

SoundFragment *FragmentDeserializer::deserialize()
{
	// try reading a 44100 16b WAV file
	try
	{
		stream in(ResourceFile::open(name, ResourceFile::SoundFragment));
		unsigned int bytecount;
		in.seek(54, stream::beg);
		in.read(&bytecount, 4);
		//TODO: fix this the proper way
		bytecount -= 96;
		in.seek(96, stream::cur);
		signed short *buf = new signed short[bytecount / sizeof(signed short)];
		in.read(buf, bytecount);
		return new SoundFragment(bytecount / sizeof(signed short), buf);
	}
	catch(...)
	{}

	// if not, assume 11025 8b SFX file
	try
	{
		stream in(ResourceFile::open(name, ResourceFile::SfxSoundFragment));
		unsigned int bytecount;
		in.seek(4, stream::beg);
		in.read(&bytecount, 4);
		in.seek(20, stream::beg);
		signed char *buf = new signed char[bytecount];
		in.read(buf, bytecount);

		signed short *buf2 = new signed short[bytecount / sizeof(signed short)];
		for (size_t i = 0; i < bytecount-1; i++)
		{
			buf2[i*4+0] = buf[i] * 0x100;
			buf2[i*4+1] = buf[i] * 0xC0 + buf[i+1] * 0x40;
			buf2[i*4+2] = buf[i] * 0x80 + buf[i+1] * 0x80;
			buf2[i*4+3] = buf[i] * 0x40 + buf[i+1] * 0xC0;
		}
		buf2[bytecount*4-4] = buf[bytecount-1] * 0x100;
		buf2[bytecount*4-3] = buf[bytecount-1] * 0x100;
		buf2[bytecount*4-2] = buf[bytecount-1] * 0x100;
		buf2[bytecount*4-1] = buf[bytecount-1] * 0x100;
		delete [] buf;

		return new SoundFragment(bytecount * 4, buf2);
	}
	catch(...)
	{}

	return NULL;
}

