#include "line.h"
#include <iostream>

Line::Line()
{
	vertices[0][3] = vertices[1][3] = 1.0f;
	INT_TO_VEC4(0xFFFFFFFF, color);
}

Line::Line(Line const &line)
{
	vec4_set(vertices[0], line.vertices[0]);
	vec4_set(vertices[1], line.vertices[1]);

	INT_TO_VEC4(0xFFFFFFFF, color);
}

Line::Line(vec2 v1, vec2 v2, vec4 col)
{
	vec2_set(vertices[0], v1);
	vec2_set(vertices[1], v2);

	vertices[0][3] = vertices[1][3] = 1.0f;

	vec4_set(color, col);

	// cout << "{ " << vertices[0][0] << ", " << vertices[0][1] << " } -> { " << vertices[1][0] << ", " << vertices[1][1] << " } " << endl;
	// cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << " A: " << color[3] << endl;
}

Line::Line(vec2 v1, vec2 v2, uint32_t col)
{
	vec2_set(vertices[0], v1);
	vec2_set(vertices[1], v2);

	vertices[0][3] = vertices[1][3] = 1.0f;

	INT_TO_VEC4(col, color);

	// cout << "{ " << vertices[0][0] << ", " << vertices[0][1] << " } -> { " << vertices[1][0] << ", " << vertices[1][1] << " } " << endl;
	// cout << "R: " << color[0] << " G: " << color[1] << " B: " << color[2] << " A: " << color[3] << endl;
}

Line::~Line()
{
	
}

int Line::isOverlapping(Line* line)
{
	ray2 r = {
		{ vertices[0][0], vertices[0][1] },
		{ vertices[1][0] - vertices[0][0], vertices[1][1] - vertices[0][1] }
	};
	vec2 intersect = {};
	float t = line->intersectedByRay(r, intersect);

	if(t > 0 && t < 1){
		return 1;
	}

	return 0;
}

static int POINT_COMPARE(const void* a, const void* b)
{
	float delta = ((vec2*)a)[0] - ((vec2*)b)[0];

	if(delta > 0) return  1;
	if(delta < 0) return -1;

	return 0;
}

int Line::bisect(vec2 point, Line lines[2])
{
	// do nothing if the point does not exist on this line
	if(!vec2_point_on_line(point, vertices[0], vertices[1])) return 0;

	lines[0] = Line(vertices[0], point,       color);
	lines[1] = Line(point,       vertices[1], color);

	// success
	return 1;
}

int Line::trim(Line line, Line newLines[2])
{
	vec2 outerMin = { vertices[0][0], vertices[1][1] }, outerMax = { line.vertices[0][0], line.vertices[1][1] };
	vec2 innerMin, innerMax;
	vec2 verts[4] = {
		{ vertices[0][0], vertices[0][1] }, 
		{ vertices[1][0], vertices[1][1] },
		{ line.vertices[0][0], line.vertices[0][1] },
		{ line.vertices[1][0], line.vertices[1][1] },
	};
	vec2 intersects[2], v1, v2;

	vec2_set(v1, vertices[0]);
	vec2_set(v2, vertices[1]);

	// find the true min and max vertices of the two lines
	vec2_rectangle_min_max(outerMin, outerMax, verts, 4);

	// printf("outerMin = {%f, %f}\n", outerMin[0], outerMin[1]);
	// printf("outerMax = {%f, %f}\n", outerMax[0], outerMax[1]);

	// Generate rays for the first line pointing from each side of the line
	// toward the opposite side
	ray2 v01 = {
		{ v1[0], v1[1] },
		{ v2[0] - v1[0], v2[1] - v1[1]}
	};

	// printf("v01.p = {%f, %f}\n", v01.p[0], v01.p[1]);
	// printf("v01.n = {%f, %f}\n", v01.n[0], v01.n[1]);
	
	ray2 v10 = {
		{ v2[0], v2[1] },
		{ v1[0] - v2[0], v1[1] - v2[1]}
	};

	// printf("v10.p = {%f, %f}\n", v10.p[0], v10.p[1]);
	// printf("v10.n = {%f, %f}\n", v10.n[0], v10.n[1]);


	// Next find each intersection point for both rays, the possible cases should look something
	// like the following
	//
	// 0              1
	// A----B====B----A t0 > 0 and t1 > 0 
	//
	// 0         1
	// A----B====A~~~~B t0 > 0 and t1 = 0 
	//
	//      0    1
	// B~~~~A====A~~~~B t0 = 0 and t1 = 0


	float t0 = line.intersectedByRay(v01, intersects[0]); 
	float t1 = line.intersectedByRay(v10, intersects[1]);

	vec2_print(intersects[0]);
	vec2_print(intersects[1]);

	vec2_set(innerMin, intersects[0]);
	vec2_set(innerMax, intersects[1]);
	vec2_rectangle_min_max(innerMin, innerMax, intersects, 2);

	if(t0 >= 0 && t1 >= 0){
		newLines[0] = Line(outerMin, innerMin, 0xFFFFFFFF);
		newLines[1] = Line(innerMax, outerMax, 0xFFFFFFFF);
		return 1;
	}

	return 0;
}

float Line::intersects(Geometry* geo, vec2 normal)
{
	return 0;
}

float Line::intersectedByRay(ray2 ray, vec2 intersect)
{
	float t;

	if(!vec2_ray_line(intersect, ray, vertices[0], vertices[1], &t)){
		return -1;
	}

	// printf("ray = {%0.3f, %0.3f} -> {%0.3f, %0.3f}\n", ray.p[0], ray.p[1], ray.n[0], ray.n[1]);
	// printf("intersects = {%0.3f, %0.3f} --- {%0.3f, %0.3f} @ t = %f\n\n", vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], t);
	return t;
}

void Line::draw(mat4x4 viewProjection)
{
	vec4 temp;

	glLineWidth(3.0);
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