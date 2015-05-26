#include "room.h"
#include <stdlib.h>
#include <iostream>

#define RAND_F ((rand() % 1024) / 1024.0)
#define FIDY_FIDY (rand() % 2)

float subRand()
{
	return RAND_F * 0.4 + 0.6;
}

Room::Room(Rectangle bounds, int depth)
{
	subdivide(&bounds, depth, NULL);
}

Room::~Room()
{

}

void Room::trace()
{
	for(int i = 0; i--;){
		Rectangle* r = regions[i];
		Line sides[4];

		r->emitLines(sides);
	}
}

void Room::subdivide(Rectangle* bounds, int recurse, vec2 required[2])
{
	Rectangle* children = new Rectangle[2];
	Rectangle *larger, *smaller;
	vec4 n[2];
	vec2 newPoints[2];

	bounds->subdivide(children, n, subRand(), FIDY_FIDY);

	vec2_set(newPoints[0], n[0]);
	vec2_set(newPoints[1], n[1]);

	if(children[0].area() > children[1].area()){
		larger = children + 0, smaller = children + 1;
	}
	else{
		larger = children + 1, smaller = children + 0;
	}

	if(recurse){
		subdivide(larger,  recurse - 1, newPoints);
		subdivide(smaller, 0,           newPoints);
	}
	else{
		for(int i = 2; i--;)
			if(larger->contains(required[i])){
				regions.push_back(larger);
				return;
			}

		for(int i = 2; i--;)
			if(smaller->contains(required[i])){
				regions.push_back(smaller);
				return;
			}
	}
}

float Room::intersects(Geometry* geo, vec2 normal)
{
	return 0;	
}

float Room::intersectedByRay(ray2 ray, vec2 intersect)
{
	return 0;
}


void Room::draw(mat4x4 viewProjection)
{
	for(int i = regions.size(); i--;){
		regions[i]->draw(viewProjection);
	}
}


void Room::setColor(uint32_t color)
{

}
