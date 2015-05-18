#pragma once

#include "geometry.h"
#include <sys/types.h>

class Line : public Geometry
{
public:
	Line(vec2 v1, vec2 v2, uint32_t color);
	~Line();

	int intersects(Geometry* geo, vec2 normal);
	int intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);

private:
	vec4 vertices[2];
	vec4 color;
};