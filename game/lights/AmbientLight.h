#ifndef AMBIENTLIGHT_H
#define AMBIENTLIGHT_H

#include "Color.h"
#include "Light.h"
#include "Frustum.h"

class AmbientLight : public Light 
{
	Color color;
	YesFrustum frustum;
public:
	AmbientLight(Color color);
};

#endif

