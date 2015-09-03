#pragma once

#include "line.h"

using namespace std;

class LineSegment : public Line
{
public:
	LineSegment(vec2 v1, vec2 v2, uint32_t color);
	LineSegment(vec2 v1, vec2 v2, vec4 color);

	void draw(mat4x4 viewProjection);
};