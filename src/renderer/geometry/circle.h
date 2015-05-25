#pragma once

#include "geometry.h"
#include <sys/types.h>

class Circle : public Geometry
{
public:
	Circle(vec2 position, float radius, uint32_t color);
	~Circle();

	float intersects(Geometry* geo, vec2 normal);
	float intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);

	void setColor(uint32_t color);

	vec4  position;
	float radius;

private:
	vec4 color;
};