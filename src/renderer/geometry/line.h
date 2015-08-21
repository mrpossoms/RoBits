#pragma once

#include "geometry.h"
#include <sys/types.h>
#include <vector>

using namespace std;

class Line : public Geometry
{
public:
	Line();
	Line(Line const &l);
	Line(float v1x, float v1y, float v2x, float v2y, uint32_t color);
	Line(vec2 v1, vec2 v2, uint32_t color);
	Line(vec2 v1, vec2 v2, vec4 color);
	Line(int fd);
	~Line();

	size_t store(int fd);

	int intersects(Geometry* geo, vec2 normal, float* t);
	int intersectedByRay(ray2 ray, vec2 intersect, float* t);

	int isOverlapping(Line* line);
	int bisect(vec2 point, Line lines[2]);

	int trim(Line line, Line newLines[2]);

	void draw(mat4x4 viewProjection);
	void setColor(uint32_t color);

	vec4 vertices[2];
private:
	vec4 color;
};