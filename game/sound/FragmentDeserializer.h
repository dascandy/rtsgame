#ifndef FRAGMENTDESERIALIZER_H
#define FRAGMENTDESERIALIZER_H

#include "SoundFragment.h"
#include <string>

class FragmentDeserializer
{
public:
	FragmentDeserializer(std::string name);
	SoundFragment *deserialize();
private:
	std::string name;
};

#endif

