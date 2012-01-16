#include "MusicTrack.h"
#include "vorbis\vorbisfile.h"
#include "stream.h"
#include "ResourceFile.h"
#include "debug.h"

struct MusicTrack::Data
{
	OggVorbis_File vf;
	stream *str;
};

MusicTrack::MusicTrack()
{
	_imp = new MusicTrack::Data();
	_imp->str = NULL;
	state = Stopped;
	ownData = (signed short *)malloc(4 * 16384);
	TODO_W("make the music pre-read in a separate thread");
}

MusicTrack::~MusicTrack()
{
	//TODO: stop using the stream
	if (state == Playing)
	{
		ov_clear(&_imp->vf);
	}

	if (_imp->str)
	{
		delete _imp->str;
		_imp->str = 0;
	}

	free(ownData);
}

void MusicTrack::mix(signed short *data, int samples)
{
	if (state == Playing)
	{
		int _streamno;
		int bytesRead = 0;
		do
		{
			long res = ov_read(&_imp->vf, ((char *)ownData)+bytesRead, samples * 4 - bytesRead, 0, 2, 1, &_streamno);
			if (res > 0)
				bytesRead += res;
		}
		while(bytesRead != samples * 4);

		for (int i = 0; i < samples*2; i++)
		{
			data[i] = ownData[i] / 8;
		}
	}
}

void MusicTrack::SetSource(const std::string &name)
{
	if (name != this->name)
	{
		if (state == Playing)
		{
			ov_clear(&_imp->vf);
		}

		if (_imp->str)
		{
			delete _imp->str;
			_imp->str = 0;
		}

		this->name = name;

		try
		{
			_imp->str = new stream(ResourceFile::open(name, ResourceFile::Music));

			ov_callbacks callbacks;
			callbacks.read_func = &MusicTrack::read;
			callbacks.seek_func = &MusicTrack::seek;
			callbacks.tell_func = &MusicTrack::tell;
			callbacks.close_func = &MusicTrack::close;

			ov_open_callbacks(this, &_imp->vf, NULL, 0, callbacks);
			state = Playing;
		}
		catch(...)
		{
			TODO_W("Invalid track name: %s\n", name.c_str());
			state = Stopped;
		}
	}
}

size_t MusicTrack::read(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	MusicTrack *track = (MusicTrack *)datasource;
	return track->_imp->str->read(ptr, size*nmemb);
}

int MusicTrack::seek(void *datasource, ogg_int64_t offset, int whence)
{
	MusicTrack *track = (MusicTrack *)datasource;
	stream::seekdir dir;
	switch(whence)
	{
	case SEEK_SET: dir = stream::beg; break;
	default:
	case SEEK_CUR: dir = stream::cur; break;
	case SEEK_END: dir = stream::end; break;
	}
	return track->_imp->str->seek((int)offset, dir);
}

int MusicTrack::close(void *datasource)
{
	MusicTrack *track = (MusicTrack *)datasource;
	delete track->_imp->str;
	track->_imp->str = 0;
	return 0;
}

long MusicTrack::tell(void *datasource)
{
	MusicTrack *track = (MusicTrack *)datasource;
	return track->_imp->str->tell();
}

