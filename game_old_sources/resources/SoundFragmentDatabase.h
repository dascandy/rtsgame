#ifndef SOUNDFRAGMENTDATABASE_H
#define SOUNDFRAGMENTDATABASE_H

#include <string>
#include <map>
class SoundFragment;

class SoundFragmentDatabase
{
public:
	SoundFragment *operator[](const std::string &index);
	~SoundFragmentDatabase();
private:
	SoundFragment *LoadFragment(std::string name);
	std::map<std::string, SoundFragment *> fragments;
};
extern SoundFragmentDatabase soundDatabase;

#endif

