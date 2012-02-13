#ifndef GAMESTATE_H
#define GAMESTATE_H

class ShaderProgram;
#include "Vector3.h"

class GameState
{
public:
	static GameState &Instance() { static GameState state; return state; }
	void SetVars(ShaderProgram &sp);
	void Update(bool active);
	bool ended;
	bool paused;
	unsigned long long gametime;
	Vector3 gamePosition;
private:
	GameState();
	~GameState() {}
	GameState(const GameState &);
	void operator=(const GameState &);
	int relGameSpeed; // 1000 == normal speed
	int lastTicks;
};

#endif

