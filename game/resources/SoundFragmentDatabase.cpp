#include "SoundFragmentDatabase.h"
#include "SoundFragment.h"
#include "ResourceFile.h"
#include "FragmentDeserializer.h"
#include "debug.h"

SoundFragmentDatabase soundDatabase;

SoundFragment *SoundFragmentDatabase::operator[](const std::string &name)
{
	std::map<std::string, SoundFragment *>::iterator it = fragments.find(name);
	if (it != fragments.end())
		return it->second;

	SoundFragment *fragment = LoadFragment(name);

	if (!fragment)
		TODO("make fragment");

	fragments.insert(make_pair(name, fragment));
	return fragment;
}

SoundFragmentDatabase::~SoundFragmentDatabase()
{
	for (std::map<std::string, SoundFragment *>::iterator it = fragments.begin(); it != fragments.end(); ++it)
	{
		delete it->second;
	}
}

SoundFragment *SoundFragmentDatabase::LoadFragment(std::string name)
{
	SoundFragment *sf = FragmentDeserializer(name).deserialize();
	return sf;
}

