#ifndef SOUNDFRAGMENT_H
#define SOUNDFRAGMENT_H

class Vector3;

// 44100hz, 16bit mono
class SoundFragment
{
public:
	SoundFragment(unsigned int sampleCount, signed short *fragmentData);
	~SoundFragment();
	signed short getSample(float index)
	{
		if (index < 0 || index > sampleCount - 1) return 0;
		float fraction = (index - (int)index);
		signed short s1 = fragmentData[(int)index],
					 s2 = fragmentData[(int)index+1];
		return (signed short)(s2 * fraction + s1 * (1-fraction));
	}
	unsigned int GetSampleCount() { return sampleCount; }
private:
	signed short *fragmentData;
	unsigned int sampleCount;
};

#endif

