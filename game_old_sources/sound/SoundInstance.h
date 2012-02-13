#ifndef SOUNDINSTANCE_H
#define SOUNDINSTANCE_H

class SoundFragment;
#include "Vector3.h"

class SoundInstance
{
public:
	SoundInstance(SoundFragment *fragment)
	: fragment(fragment)
	, currentIndex(0)
	{
	}
	virtual bool mix(signed short *data, int samples) = 0;
protected:
	SoundFragment *fragment;
	unsigned int currentIndex;
};

class SoundGlobalInstance : public SoundInstance
{
public:
	SoundGlobalInstance(SoundFragment *fragment) 
	: SoundInstance(fragment)
	{
	}
	bool mix(signed short *data, int samples);
};

class SoundPositionalInstance : public SoundInstance
{
public:
	SoundPositionalInstance(SoundFragment *fragment, const Vector3 &vec, float volume)
	: SoundInstance(fragment)
	, vec(vec)
	, volume(volume)
	{
	}
	bool mix(signed short *data, int samples);
private:
	Vector3 vec;
	float volume;
};

class SoundRepeatInstance : public SoundInstance
{
public:
	SoundRepeatInstance(SoundFragment *fragment)
	: SoundInstance(fragment)
	, stopped(false)
	{
	}
	void Stop()
	{
		stopped = true;
	}
protected:
	bool stopped;
};

class SoundPositionalRepeatInstance : public SoundRepeatInstance
{
public:
	SoundPositionalRepeatInstance(SoundFragment *fragment, const Vector3 &vec, float volume)
	: SoundRepeatInstance(fragment)
	, vec(vec)
	, volume(volume)
	{
	}
	bool mix(signed short *data, int samples);
private:
	Vector3 vec;
	float volume;
};

class SoundMovingRepeatInstance : public SoundRepeatInstance
{
public:
	SoundMovingRepeatInstance(SoundFragment *fragment, const Vector3 &vec, const Vector3 &dir)
	: SoundRepeatInstance(fragment)
	, vec(vec)
	, dir(dir)
	{
	}
	bool mix(signed short *data, int samples);
private:
	Vector3 vec;
	Vector3 dir;
};

#endif

