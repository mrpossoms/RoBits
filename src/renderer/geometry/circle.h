#pragma once

#include "geometry.h"
#include <sys/types.h>

class Circle : public Geometry
{
public:
	Circle(const vec2 position, float radius, uint32_t color);
	~Circle();

	size_t store(int fd);

	int intersects(Geometry* geo, vec2 normal, float* t);
	int intersectedByRay(ray2 ray, vec2 intersect, float* t);

	void draw(mat4x4 viewProjection);

	void setColor(uint32_t color);

	vec4  position;
	float radius;

private:
	vec4 color;
};