#ifndef MODELDESERIALIZERMS3D_H
#define MODELDESERIALIZERMS3D_H

#include <string>
#include <vector>
#include <map>

#include "stream.h"
#include "Model.h"

class ModelDeserializerMs3d
{
public:
	ModelDeserializerMs3d(std::string filename);
	Model *deserialize();
private:
	void deserializeGroup();
	void deserializePoint3D();
	void deserializeTriangle();
	void Smooth();
	std::vector<Model::vertex> points;
	std::vector<Model::vertex> vertices;
	std::map<Model::vertex, unsigned short> smoothingGroups;
	unsigned short nextSmoothingGroup;

	stream os;
};

#endif

