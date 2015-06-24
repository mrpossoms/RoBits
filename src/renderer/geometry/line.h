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
	Line(vec2 v1, vec2 v2, uint32_t color);
	Line(vec2 v1, vec2 v2, vec4 color);
	~Line();

	float intersects(Geometry* geo, vec2 normal);
	float intersectedByRay(ray2 ray, vec2 intersect);

	int isOverlapping(Line* line);
	int bisect(vec2 point, Line lines[2]);

	int trim(Line line, Line newLines[2]);

	void draw(mat4x4 viewProjection);
	void setColor(uint32_t color);

	vec4 vertices[2];
private:
	vec4 color;
};