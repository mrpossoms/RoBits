#include "circle.h"
#include "line.h"
#include <iostream>

using namespace std;

Circle::Circle(const vec2 pos, float r, uint32_t col)
{
	vec2_set(position, (float*)pos);
	position[3] = 1.0f;
	radius = r;

	INT_TO_VEC4(col, color);
}

size_t Circle::store(int fd)
{
	size_t size = 0;

	size += write(fd, position, sizeof(vec2));
	size += write(fd, &radius, sizeof(float));
	size += write(fd, color, sizeof(vec4));

	return size;
}

Circle::~Circle()
{
	
}

int Circle::intersects(Geometry* geo, vec2 normal, float* t)
{
	Line* line = dynamic_cast<Line*>(geo);

	if(line){ // line circle intersection
		ray2 ray = {
			// position
			{ line->vertices[0][0], line->vertices[0][1] },
			// direction
			{
				line->vertices[1][0] - line->vertices[0][0],
				line->vertices[1][1] - line->vertices[0][1]
			}
		};
		vec2 intersect = {};

		float t;
		if(this->intersectedByRay(ray, intersect, &t)){
			return 0 <= t && t <= 1;
		}
	}


	return 0;
}

int Circle::intersectedByRay(ray2 ray, vec2 intersect, float* t)
{
	float px = ray.p[0] - position[0], py = ray.p[1] - position[1];
	float nx = ray.n[0], ny = ray.n[1];

	float a = nx * nx + ny * ny;
	float b = 2 * (px * nx + py * ny);
	float c = px * px + py * py - radius * radius;

	float rad = sqrt(b * b - 4 * a * c);
	float _2a  = 2 * a;
	float t1 = (-b + rad) / _2a;
	float t2 = (-b - rad) / _2a;

	if(!isnan(t1) && t1 < t2){
		intersect[0] = ray.p[0] + ray.n[0] * t1;
		intersect[1] = ray.p[1] + ray.n[1] * t1;
		*t = t1;

		return 1;
	}
	else if(!isnan(t2)){
		intersect[0] = ray.p[0] + ray.n[0] * t2;
		intersect[1] = ray.p[1] + ray.n[1] * t2;	
		*t = t2;

		return 1;
	}

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

void Circle::setColor(uint32_t col)
{
	INT_TO_VEC4(col, color);
}
