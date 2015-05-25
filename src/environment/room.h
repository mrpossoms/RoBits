#pragma once

#include "geometry.h"
#include "geometry/Line.h"
#include "geometry/Rectangle.h"

#include <sys/types.h>
#include <vector>

using namespace std;

class Room : public Geometry
{
public:
	Room(Rectangle bounds, int depth);
	~Room();

	float intersects(Geometry* geo, vec2 normal);
	float intersectedByRay(ray2 ray, vec2 intersect);

	void draw(mat4x4 viewProjection);

	void setColor(uint32_t color);

private:
	void subdivide(Rectangle* bounds, int recurse);

	vector<Geometry*> features;
	vector<Rectangle*> regions;
};