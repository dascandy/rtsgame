#ifndef KEYMAP_H
#define KEYMAP_H

#include <map>

enum GameKey 
{
	None,
	RunForward,
	MoveForward,
	StepLeft,
	StepRight,
	StepForward,
	StepBackward,
	RollLeft,
	RollRight,
	RotateLeft,
	RotateRight,
	StandingJump,
	Crouch,
	Shoot,
	NextGun,
	PreviousGun,
	NextInv,
	PreviousInv,
	UseInv,
	SelectItem,
	UseItem,
	UseHealthPack,
	UseEnergyCube,
};

enum Modifier
{
	Ctrl = 0x200,
	Alt = 0x400,
	Shift = 0x800,
};

class Keymap
{
	bool lShift, rShift,
		 lAlt, rAlt,
		 lCtrl, rCtrl;
public:
	std::map<int, GameKey> translationTable;
	Keymap();
	GameKey translate(int scancode);
};

#endif

