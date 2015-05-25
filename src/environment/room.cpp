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
	subdivide(&bounds, depth);
}

Room::~Room()
{

}

void Room::subdivide(Rectangle* bounds, int recurse)
{
	Rectangle* children = new Rectangle[2];
	Rectangle *larger, *smaller;
	bounds->subdivide(children, subRand(), FIDY_FIDY);

	if(children[0].area() > children[1].area()){
		larger  = children + 0;
		smaller = children + 1;
	}
	else{
		larger  = children + 1;
		smaller = children + 0;		
	}

	if(!recurse){
		regions.push_back(larger);
	}
	else
	{
		subdivide(larger, recurse - 1);
		subdivide(smaller, 0);
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
