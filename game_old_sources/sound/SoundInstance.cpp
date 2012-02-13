#include "SoundInstance.h"
#include "SoundFragment.h"
#include "debug.h"
#include "Scene.h"
#include "Object.h"
#include "Bone.h"
#include "GameState.h"

bool SoundGlobalInstance::mix(signed short *data, int samples)
{
	for (int i = 0; i < samples; i++)
	{
		if (currentIndex >= fragment->GetSampleCount())
			return true;
		data[i*2+0] += fragment->getSample((float)currentIndex) / 16;
		data[i*2+1] += fragment->getSample((float)currentIndex) / 16;
		currentIndex++;
	}
	return false;
}

bool SoundPositionalInstance::mix(signed short *data, int samples)
{
	float dist = ((GameState::Instance().gamePosition - vec).len() + 25) / 25;
	float currentVol = volume / (16 * dist * dist);
	for (int i = 0; i < samples; i++)
	{
		if (currentIndex >= fragment->GetSampleCount())
			return true;
		data[i*2+0] += (short)(fragment->getSample((float)currentIndex) * currentVol);
		data[i*2+1] += (short)(fragment->getSample((float)currentIndex) * currentVol);
		currentIndex++;
	}
	return false;
}

bool SoundPositionalRepeatInstance::mix(signed short *data, int samples)
{
	if (stopped) 
		return true;

	float dist = ((GameState::Instance().gamePosition - vec).len() + 25) / 25;
	float currentVol = volume / (16 * dist * dist);
	for (int i = 0; i < samples; i++)
	{
		if (currentIndex >= fragment->GetSampleCount())
			currentIndex = 0;
		data[i*2+0] += (short)(fragment->getSample((float)currentIndex) * currentVol);
		data[i*2+1] += (short)(fragment->getSample((float)currentIndex) * currentVol);
		currentIndex++;
	}
	return false;
}

bool SoundMovingRepeatInstance::mix(signed short *, int )
{
	TODO("make %s", __FUNCTION__);
	return false;
}

