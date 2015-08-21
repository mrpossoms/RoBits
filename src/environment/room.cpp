#include "room.h"
#include <stdlib.h>
#include <iostream>

#define FIDY_FIDY (rand() % 2)

float subRand()
{
	return RAND_F * 0.4 + 0.3;
}

Room::Room(Rectangle bounds, int depth)
{
	subdivide(&bounds, depth, NULL);
	trace();
}

Room::Room(int fd)
{
	uint32_t edges;

	read(fd, &edges, sizeof(uint32_t));
	cout << "Loading " << edges << " walls\n";

	for(int i = edges; i--;){
		perimeter.push_back(new Line(fd));
	}
}

Room::~Room()
{

}

size_t Room::store(int fd)
{
	size_t size = 0;

	for(int i = perimeter.size(); i--;){
		size += perimeter[i]->store(fd);
	}

	return size;
}

vector<Rectangle*>* Room::neighborsOf(Rectangle* rectangle)
{
	vector<Rectangle*>* n = new vector<Rectangle*>();

	for(int i = regions.size(); i--;){
		if(rectangle == regions[i]) continue;

		for(int j = 4; j--;){
			if(regions[i]->contains(rectangle->vertices[j]) || rectangle->contains(regions[i]->vertices[j])){
				n->push_back(regions[i]);
				break;
			}
		}
	}

	return n;
}

void Room::trace()
{
	for(int i = regions.size(); i--;){
		perimeter.push_back(regions[i]);
	}

	for(int i = regions.size(); i--;){
		Rectangle* me = regions[i];
		vector<Rectangle*>* neighbors = neighborsOf(me);
		Line lines[4];
		Line myLines[4];

		me->emitLines(myLines);

		printf("%lx -> { ", (unsigned long)regions[i]);
		for(int j = neighbors->size(); j--;){
			printf("%lx ", (unsigned long)(*neighbors)[j]);
		}printf("}\n");

		for(int k = 4, trimmed = 0; k--;){
				
			// printf("%lx -> { ", (unsigned long)regions[i]);
			for(int j = neighbors->size(); j--;){
				Rectangle* neighbor = (*neighbors)[j];
				if(neighbor == me) continue;

				neighbor->emitLines(lines);
				
				for(int l = 4; l--;){
					Line newLines[2];
					if(myLines[k].isOverlapping(lines + l)){
						// myLines[k].trim(lines[l], newLines);
						// perimeter.push_back(new Line(newLines[0]));
						// perimeter.push_back(new Line(newLines[1]));
						perimeter.push_back(new Line(myLines[k].vertices[0], myLines[k].vertices[1], 0x0000FFFF));
						trimmed = 1;
						// break;
					}
				}
			}
			// 	printf("%lx ", (unsigned long)(*neighbors)[j]);
				
			if(!trimmed){
				// perimeter.push_back(new Line(myLines[k]));
			}
		}

		delete neighbors;		
	}

	 
}

void Room::subdivide(Rectangle* bounds, int recurse, vec2 required[2])
{
	Rectangle* children = new Rectangle[2];
	Rectangle *larger, *smaller;
	vec4 n[2];
	vec2 newPoints[2];

	float p = subRand();
	printf("Subdivide p = %f\n", p);

	bounds->subdivide(children, n, p, FIDY_FIDY);

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

int Room::intersects(Geometry* geo, vec2 normal, float* t)
{
	return 0;	
}

int Room::intersectedByRay(ray2 ray, vec2 intersect, float* t)
{
	return 0;
}


void Room::draw(mat4x4 viewProjection)
{
	glLineWidth(3.0);
	glBegin(GL_LINES);

	for(int i = perimeter.size(); i--;){
		perimeter[i]->draw(viewProjection);
	}

	glEnd();
}


void Room::setColor(uint32_t color)
{

}
