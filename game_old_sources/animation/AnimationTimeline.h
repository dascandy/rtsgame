#ifndef ANIMATIONTIMELINE_H
#define ANIMATIONTIMELINE_H

class AnimationTimeline
{
public:
	bool SetAnimationTime(int time);
private:
	std::vector<AnimationEffect *> properties;
	std::vector<std::pair<int, RenderView *> > cameramotion;
	int duration, current;
};

#endif

