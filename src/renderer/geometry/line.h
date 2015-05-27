#pragma once

#include "geometry.h"
#include <sys/types.h>

class Line : public Geometry
{
public:
	Line();
	Line(vec2 v1, vec2 v2, uint32_t color);
	~Line();

	float intersects(Geometry* geo, vec2 normal);
	float intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);
	void setColor(uint32_t color);

	vec4 vertices[2];
private:
	vec4 color;
};