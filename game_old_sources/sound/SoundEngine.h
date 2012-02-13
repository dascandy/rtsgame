#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <string>
#include <vector>
class SoundInstance;
class SoundRepeatInstance;
class SoundFragment;
class Vector3;
class Movie;

#include "MusicTrack.h"

class SoundEngine
{
public:
	static SoundEngine &Instance() { static SoundEngine engine; return engine; }
	SoundEngine();
	~SoundEngine();
	static void DoMixAudio(void *baseptr, unsigned char *stream, int len);
	void MixAudio(signed short *stream, int len);
	void PlaySoundGlobal(SoundFragment *fragment);
	void PlaySoundPositional(SoundFragment *fragment, const Vector3 &loc, float volume);
	SoundRepeatInstance *PlaySoundPositionalC(SoundFragment *fragment, const Vector3 &loc, float volume);
	SoundRepeatInstance *PlaySoundMovingC(SoundFragment *fragment, const Vector3 &loc, const Vector3 &dir, float volume);
	void SetMusicTrack(const std::string &name);
	void SetMovie(Movie *movie);
private:
	Movie *activeMovie;
	MusicTrack track;
	std::vector<SoundInstance *> sounds;
};

#endif

