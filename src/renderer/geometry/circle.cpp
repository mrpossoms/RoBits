#include "circle.h"
#include <iostream>

using namespace std;

Circle::Circle(vec2 pos, float r, uint32_t col)
{
	vec2_set(position, pos);
	position[3] = 1.0f;
	radius = r;

	INT_TO_VEC4(col, color);

	cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << endl;
}

Circle::~Circle()
{
	
}

int Circle::intersects(Geometry* geo, vec2 normal)
{
	return 0;
}

int Circle::intersectedByRay(ray2 ray, vec2 intersect)
{
	return 0;
}

void Circle::draw(mat4x4 viewProjection)
{
	vec4 temp;

	glBegin(GL_LINE_LOOP);

	glColor4f(color[0], color[1], color[2], color[3]);	

	float dt = 2 * M_PI / 32.0f;
	for(int i = 32; i--;){
		float t = i * dt;
		vec4 display;

		temp[0] = position[0] + radius * cosf(t);
		temp[1] = position[1] + radius * sinf(t);
		temp[2] = 0;
		temp[3] = 1;

		mat4x4_mul_vec4(display, viewProjection, temp);

		glVertex2f(display[0], display[1]);
	}

	glEnd();
}