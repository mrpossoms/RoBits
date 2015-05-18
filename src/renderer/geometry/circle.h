#pragma once

#include "geometry.h"
#include <sys/types.h>

class Circle : public Geometry
{
public:
	Circle(vec2 position, float radius, uint32_t color);
	~Circle();

	int intersects(Geometry* geo, vec2 normal);
	int intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);

	vec4  position;
	float radius;

private:
	vec4 color;
};