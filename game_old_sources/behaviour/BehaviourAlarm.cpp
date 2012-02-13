#include "BehaviourAlarm.h"
#include "Behaviour.h"
#include "SoundEngine.h"
#include "SoundFragmentDatabase.h"
#include <assert.h>

DEFINE_BEHAVIOUR_TYPE(Alarm, 2, 2)

std::string BehaviourTypeAlarm::
getTriggerName(int no)
{
	switch(no)
	{
	case 0: return "AlarmTrigger";
	case 1: return "AlarmCancel";
	default: assert(false); exit(-1);
	}
}

std::string BehaviourTypeAlarm::getEventName(int no)
{
	switch(no)
	{
	case 0: return "AlarmActive";
	case 1: return "AlarmInactive";
	default: assert(false); exit(-1);
	}
}

BehaviourAlarm::BehaviourAlarm(Object *obj, std::map<std::string, std::string> &args) 
: ActiveBehaviour(obj, 20000)
, Alarm(soundDatabase[args["alarmsound"]])
, CodeRedTerminated(soundDatabase["CodeRedTerminated"])
, AreaSecure(soundDatabase["AreaSecure"])
, AlarmHasBeenDeactivated(soundDatabase["AlarmHasBeenDeactivated"])
, SecurityHasBeenBreached(soundDatabase["SecurityHasBeenBreached"])
, alarmActive(false)
{
	DefineEvent(AlarmActive);
	DefineEvent(AlarmInactive);
}

void BehaviourAlarm::UpdateFrame()
{
	if (alarmActive) 
	{
		framesUntilSecurityBreach--;
		if (!framesUntilSecurityBreach)
		{
			SoundEngine::Instance().PlaySoundGlobal(SecurityHasBeenBreached);
			framesUntilSecurityBreach = FramesUntilSecurityBreach;

			// do anything with this?
		}
	}
}

void BehaviourAlarm::AlarmTrigger()
{
	if (!alarmActive)
	{
		alarmActive = true;
		AlarmActive();
		SoundEngine::Instance().PlaySoundGlobal(Alarm);
	}
}

void BehaviourAlarm::AlarmCancel()
{
	if (alarmActive)
	{
		alarmActive = false;
		AlarmInactive();
		switch(alarmSoundRand.getNextI(3))
		{
		case 0: SoundEngine::Instance().PlaySoundGlobal(CodeRedTerminated); break;
		case 1: SoundEngine::Instance().PlaySoundGlobal(AreaSecure); break;
		case 2: SoundEngine::Instance().PlaySoundGlobal(AlarmHasBeenDeactivated); break;
		}
	}
}

