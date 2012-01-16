#ifndef GROUND_H
#define GROUND_H

class Model;
class Texture;
class ShaderProgram;

class Ground {
public:
	Ground();
	void Draw(ShaderProgram &prog);
	Model *model;
private:
	void generateModel();
	float *ground;
	void *physicsHandle;
	Texture *tex;
	Texture *heights;
	int w, h;
};

#endif

