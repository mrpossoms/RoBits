#include "line.h"
#include <iostream>

using namespace std;

Line::Line(vec2 v1, vec2 v2, uint32_t col)
{
	vec2_set(vertices[0], v1);
	vec2_set(vertices[1], v2);

	vertices[0][3] = vertices[1][3] = 1.0f;

	INT_TO_VEC4(col, color);

	cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << endl;
}

Line::~Line()
{
	
}

float Line::intersects(Geometry* geo, vec2 normal)
{
	return 0;
}

float Line::intersectedByRay(ray2 ray, vec2 intersect)
{
	return vec2_ray_line(intersect, ray, vertices[0], vertices[1]);
}

void Line::draw(mat4x4 viewProjection)
{
	vec4 temp;

	glBegin(GL_LINES);

	glColor4f(color[0], color[1], color[2], color[3]);

	mat4x4_mul_vec4(temp, viewProjection, vertices[0]);
	glVertex2f(temp[0], temp[1]);
	mat4x4_mul_vec4(temp, viewProjection, vertices[1]);
	glVertex2f(temp[0], temp[1]);

	glEnd();
}

void Line::setColor(uint32_t col)
{
	INT_TO_VEC4(col, color);
}