#ifndef MODEL_H
#define MODEL_H

#include "Renderable.h"
#include "VertexBuffer.h"
#include <string>
#include "ResourceManager.h"

class ShaderProgram;
class PhysicsModel;
class stream;

class Model : public Renderable
{
public:
	~Model() {}
	void Draw(Res<ShaderProgram> &prog);
	static Model &Load(const std::string &name);
	static Model &Raster();
	static Model &Square();
	static Model &Cube();
	struct vertex
	{
		float x, y, z;
		float nx, ny, nz;
		float s0, t0;
		unsigned char boneId, materialType, smoothId, pad;
		int pad2[7];	// this is such a waste...
		bool operator<(const vertex &o) const
		{
			if (o.x > x) return true;
			if (o.x < x) return false;
			if (o.y > y) return true;
			if (o.y < y) return false;
			if (o.z > z) return true;
			if (o.z < z) return false;
			if (o.nx > nx) return true;
			if (o.nx < nx) return false;
			if (o.ny > ny) return true;
			if (o.ny < ny) return false;
			if (o.nz > nz) return true;
			if (o.nz < nz) return false;
			if (o.s0 > s0) return true;
			if (o.s0 < s0) return false;
			if (o.t0 > t0) return true;
			if (o.t0 < t0) return false;
			if (o.boneId > boneId) return true;
			if (o.boneId < boneId) return false;
			if (o.materialType > materialType) return true;
			if (o.materialType < materialType) return false;
			return false;
		}
	};
	void PreTranslate(PhysicsModel *m);
private:
	friend class Ground;
	friend class Font;
	friend class Menu;
	Model(int vertexcount, vertex *vertices);
	void Reload();
	static Model &loadFromStream(stream file);
	VertexBuffer vb;
};

#endif

