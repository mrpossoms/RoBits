#include "rectangle.h"
#include <strings.h>
#include <iostream>
#include <math.h>

using namespace std;

Rectangle::Rectangle()
{
	bzero(vertices, sizeof(vertices));
	for(int i = 4; i--; vertices[i][3] = 1.0);
}

Rectangle::Rectangle(vec2 ul, vec2 lr, uint32_t col)
{
	bzero(vertices, sizeof(vertices));

	vec2 ur = { lr[0], ul[1] };
	vec2 ll = { ul[0], lr[1] };

	vec2_set(vertices[0], ul);
	vec2_set(vertices[1], ur);
	vec2_set(vertices[2], lr);
	vec2_set(vertices[3], ll);

	for(int i = 4; i--; vertices[i][3] = 1.0);

	setColor(col);
}

Rectangle::~Rectangle()
{

}

float Rectangle::intersects(Geometry* geo, vec2 normal)
{
	return 0;
}

float Rectangle::intersectedByRay(ray2 ray, vec2 intersect)
{
	float t = 100000000;

	for(int i = 3; i--;){
		if(vec2_ray_line(intersect, ray, vertices[i], vertices[i+1])){
			vec2 delta; vec2_sub(delta, intersect, ray.p);
			float temp = vec2_len(delta);

			if(temp > 0 && temp < t) t = temp;
		}
	}

	return t;
}

void Rectangle::draw(mat4x4 viewProjection)
{
	vec4 temp;

	//glBegin(GL_LINE_LOOP);
	glBegin(GL_QUADS);

	glColor4f(color[0], color[1], color[2], color[3]);

	mat4x4_mul_vec4(temp, viewProjection, vertices[0]);
	glVertex2f(temp[0], temp[1]);
	mat4x4_mul_vec4(temp, viewProjection, vertices[1]);
	glVertex2f(temp[0], temp[1]);
	mat4x4_mul_vec4(temp, viewProjection, vertices[2]);
	glVertex2f(temp[0], temp[1]);
	mat4x4_mul_vec4(temp, viewProjection, vertices[3]);
	glVertex2f(temp[0], temp[1]);

	glEnd();
}

void Rectangle::setColor(uint32_t col)
{
	INT_TO_VEC4(col, color);
}

void Rectangle::subdivide(
	Rectangle children[2],
	vec4 boundryPoints[2],
	float p,
	int isVertical)
{
	vec4 *from1, *to1;
	vec4 *from2, *to2;

	if(isVertical){
		from1 = vertices + 3, to1 = vertices;
		from2 = vertices + 2, to2 = vertices + 1;
	}
	else{
		from1 = vertices, to1 = vertices + 1;
		from2 = vertices + 3, to2 = vertices + 2;
	}

	vec4_lerp(boundryPoints[0], *from1, *to1, p);
	vec4_lerp(boundryPoints[1], *from2, *to2, p);

	if(isVertical){
		Rectangle r1(boundryPoints[0], vertices[2], 0xFF0000FF);
		Rectangle r2(vertices[0], boundryPoints[1], 0xFF0000FF);

		children[0] = r1;
		children[1] = r2;
	}
	else{
		Rectangle r1(vertices[0], boundryPoints[1], 0xFF0000FF);
		Rectangle r2(boundryPoints[0], vertices[2], 0xFF0000FF);

		children[0] = r1;
		children[1] = r2;
	}
}

void Rectangle::subdivide(Rectangle children[2], float p, int isVertical)
{
	vec4 boundryPoints[2];
	subdivide(children, boundryPoints, p, isVertical);
}

float Rectangle::area()
{
	vec4 delta = {};
	vec4_sub(delta, vertices[0], vertices[2]);

	return delta[0] * delta[1];
}

float Rectangle::width()
{
	vec4 delta = {};
	vec4_sub(delta, vertices[0], vertices[2]);

	return fabs(delta[0]);
}

float Rectangle::height()
{
	vec4 delta = {};
	vec4_sub(delta, vertices[0], vertices[2]);

	return fabs(delta[1]);
}

int Rectangle::contains(float x, float y){
	vec2 v = { x, y };
	return contains(v);
}

int Rectangle::contains(vec2 position){
	vec2 ul = { vertices[0][0], vertices[0][1] };
	vec3 lr = { vertices[1][0], vertices[1][1] };

	for(int i = 4; i--;){
		vec4 v;
		vec4_set(v, vertices[i]);
		ul[0] = ul[0] < v[0] ? ul[0] : v[0];
		ul[1] = ul[1] < v[1] ? ul[1] : v[1];

		lr[0] = lr[0] > v[0] ? lr[0] : v[0];
		lr[1] = lr[1] > v[1] ? lr[1] : v[1];
	}

	if(!(position[0] >= ul[0] && position[0] <= lr[0])) return 0;
	if(position[1] >= ul[1] && position[1] <= lr[1])    return 1;

	return 0;
}

void Rectangle::emitLines(Line sides[4])
{
	// top
	vec4_set(sides[0].vertices[0], vertices[0]);
	vec4_set(sides[0].vertices[1], vertices[1]);

	// right
	vec4_set(sides[1].vertices[0], vertices[1]);
	vec4_set(sides[1].vertices[1], vertices[2]);

	// bottom
	vec4_set(sides[2].vertices[0], vertices[2]);
	vec4_set(sides[2].vertices[1], vertices[3]);

	// left
	vec4_set(sides[3].vertices[0], vertices[3]);
	vec4_set(sides[3].vertices[1], vertices[0]);
}
