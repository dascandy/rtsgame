#include "SoundEngine.h"
#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include "SoundInstance.h"
#include "Vector3.h"
#include "debug.h"
#include "SoundFragment.h"

SoundEngine::SoundEngine()
{
    SDL_AudioSpec fmt;

    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;
    fmt.callback = DoMixAudio;
    fmt.userdata = this;

    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        LOG_WARN("Unable to open audio: %s", SDL_GetError());
		throw std::exception();
	}
    SDL_PauseAudio(0);
}

SoundEngine::~SoundEngine()
{
    SDL_CloseAudio();
}

void SoundEngine::DoMixAudio(void *baseptr, unsigned char *stream, int len)
{
	SoundEngine *engine = (SoundEngine *)baseptr;
	engine->MixAudio((signed short*)stream, len/4);
}

void SoundEngine::MixAudio(signed short *stream, int samples)
{
	memset(stream, 0, samples * 4);

	track.mix(stream, samples);
	for (size_t index = 0; index < sounds.size(); ++index)
	{
		bool done = sounds[index]->mix(stream, samples);
		if (done)
		{
			delete sounds[index];
			sounds[index] = sounds.back();
			sounds.pop_back();
			index--;
		}
	}
}

void SoundEngine::SetMusicTrack(const std::string &name)
{
    SDL_LockAudio();
	track.SetSource(name);
    SDL_UnlockAudio();
}

void SoundEngine::PlaySoundGlobal(SoundFragment *fragment)
{
	SoundInstance *inst = new SoundGlobalInstance(fragment);
    SDL_LockAudio();
	sounds.push_back(inst);
    SDL_UnlockAudio();
}

void SoundEngine::PlaySoundPositional(SoundFragment *fragment, const Vector3 &vec, float volume)
{
	SoundInstance *inst = new SoundPositionalInstance(fragment, vec, volume);
    SDL_LockAudio();
	sounds.push_back(inst);
    SDL_UnlockAudio();
}

SoundRepeatInstance *SoundEngine::PlaySoundPositionalC(SoundFragment *fragment, const Vector3 &vec, float volume)
{
	SoundRepeatInstance *inst = new SoundPositionalRepeatInstance(fragment, vec, volume);
    SDL_LockAudio();
	sounds.push_back(inst);
    SDL_UnlockAudio();
	return inst;
}

SoundRepeatInstance *SoundEngine::PlaySoundMovingC(SoundFragment *fragment, const Vector3 &vec, const Vector3 &dir, float)
{
	SoundRepeatInstance *inst = new SoundMovingRepeatInstance(fragment, vec, dir);
    SDL_LockAudio();
	sounds.push_back(inst);
    SDL_UnlockAudio();
	return inst;
}

