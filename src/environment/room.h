#pragma once

#include "geometry.h"
#include "geometry/line.h"
#include "geometry/rectangle.h"

#include <sys/types.h>
#include <vector>

using namespace std;

class Room : public Geometry
{
public:
	Room(Rectangle bounds, int depth);
	Room(int fd);
	~Room();

	size_t store(int fd);

	int intersects(Geometry* geo, vec2 normal, float* t);
	int intersectedByRay(ray2 ray, vec2 intersect, float* t);

	void draw(mat4x4 viewProjection);

	void setColor(uint32_t color);

	vector<Geometry*> perimeter;
private:
	vector<Rectangle*>* neighborsOf(Rectangle* rectangle);
	void subdivide(Rectangle* bounds, int recurse, vec2 required[2]);
	void trace();

	vector<Geometry*> features;
	vector<Rectangle*> regions;
};
