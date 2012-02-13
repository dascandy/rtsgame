#ifndef INPUTCALLBACK_H
#define INPUTCALLBACK_H

class InputCallback {
public:
	virtual void MouseMove(int, int) {}
	virtual void MouseMoveX(int) {}
	virtual void MouseMoveY(int) {}
	virtual void MouseScrollUp() {}
	virtual void MouseScrollDown() {}
	virtual void MouseButtonUp(int) {}
	virtual void MouseButtonDown(int) {}
	virtual void KeyDown(int) {}
	virtual void KeyUp(int) {}
};

#endif

