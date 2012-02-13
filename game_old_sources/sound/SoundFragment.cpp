#include "SoundFragment.h"
#include <stdio.h>

SoundFragment::SoundFragment(unsigned int sampleCount, signed short *fragmentData)
: sampleCount(sampleCount)
, fragmentData(fragmentData)
{
}

SoundFragment::~SoundFragment() 
{
	delete [] fragmentData;
}

