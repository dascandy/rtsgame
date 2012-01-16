#ifndef BEHAVIOURALARM_H
#define BEHAVIOURALARM_H

#include "BehaviourType.h"
#include "Random.h"
class SoundInstance;
class SoundFragment;

ACTIVE_BEHAVIOUR_CLASS(Alarm)
{
public:
	BehaviourAlarm(Object *obj, std::map<std::string, std::string> &);
	void UpdateFrame();
	void AlarmTrigger();
	void AlarmCancel();
private:
	SoundFragment *Alarm;
	SoundFragment *CodeRedTerminated;
	SoundFragment *AreaSecure;
	SoundFragment *AlarmHasBeenDeactivated;
	SoundFragment *SecurityHasBeenBreached;
	event AlarmActive;
	event AlarmInactive;
	bool alarmActive;
	Random alarmSoundRand;
	int framesUntilSecurityBreach;
	static const int FramesUntilSecurityBreach = 1800; // 30 seconds
};

#endif

