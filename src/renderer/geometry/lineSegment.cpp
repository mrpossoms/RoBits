#include "lineSegment.h"


LineSegment::LineSegment(vec2 v1, vec2 v2, vec4 col)
{
	vec2_set(vertices[0], v1);
	vec2_set(vertices[1], v2);

	vertices[0][3] = vertices[1][3] = 1.0f;

	vec4_set(color, col);

	// cout << "{ " << vertices[0][0] << ", " << vertices[0][1] << " } -> { " << vertices[1][0] << ", " << vertices[1][1] << " } " << endl;
	// cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << " A: " << color[3] << endl;
}

LineSegment::LineSegment(vec2 v1, vec2 v2, uint32_t col)
{
	vec2_set(vertices[0], v1);
	vec2_set(vertices[1], v2);

	vertices[0][3] = vertices[1][3] = 1.0f;

	INT_TO_VEC4(col, color);

	// cout << "{ " << vertices[0][0] << ", " << vertices[0][1] << " } -> { " << vertices[1][0] << ", " << vertices[1][1] << " } " << endl;
	// cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << " A: " << color[3] << endl;
}

void LineSegment::draw(mat4x4 viewProjection)
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