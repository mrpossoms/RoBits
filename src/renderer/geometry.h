#pragma once

#include "linalg.h"

#define INT_TO_VEC4(i, v){\
	v[0] = ((0xFF000000 & i) >> 24) / 255.0f;\
	v[1] = ((0x00FF0000 & i) >> 16) / 255.0f;\
	v[2] = ((0x0000FF00 & i) >> 8)  / 255.0f;\
	v[3] = ((0x000000FF & i) >> 0)  / 255.0f;\
}\

class Geometry;
class Geometry
{
public:
	virtual int intersects(Geometry* geo, vec2 normal) = 0;
	virtual int intersectedByRay(ray2 ray, vec2 intersect) = 0;

	virtual void draw(mat4x4 viewProjection) = 0;
protected:

};