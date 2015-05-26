#pragma once

#include "geometry.h"
#include "geometry/line.h"
#include <sys/types.h>

#define RECT_SUB_HORIZONTAL 0
#define RECT_SUB_VERTICAL   1

class Rectangle : public Geometry
{
public:
	Rectangle();
	Rectangle(vec2 upperLeft, vec2 lowerRight, uint32_t color);
	~Rectangle();

	float intersects(Geometry* geo, vec2 normal);
	float intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);
	void setColor(uint32_t color);


	void subdivide(Rectangle children[2], float p, int isVertical);
	void subdivide(Rectangle children[2], vec4 boundryPoints[2], float p, int isVertical);

	float area();
	float width();
	float height();

	int contains(float x, float y);
	int contains(vec2 position);

	void emitLines(Line sides[4]);

	vec4 vertices[4];
private:
	vec4 color;
};