#ifndef Scene_H
#define Scene_H

#include <vector>
#include <map>
#include <set>
#include "Renderable.h"

class Object;
class Ground;

class Scene : public Renderable {
public:
	Scene();
	void events(unsigned int milliseconds);

	void AddObject(Object *);
	void RemoveObject(Object *);
	void Draw(Res<ShaderProgram> &prog);
	void DrawPick(Res<ShaderProgram> &prog, std::vector<Object *> &objs);
	void Update();
private:
	std::vector<Object *> objects;
};

#endif

