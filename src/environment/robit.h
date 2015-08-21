#pragma once

#include "geometry.h"
#include "geometry/line.h"
#include "room.h"
#include <vector>

using namespace std;

class Robit{
public:
	Robit(Room* room);
	~Robit();

	int intersects(Geometry* geo, vec2 normal, float* t);
	void draw(mat4x4 viewProjection);
private:
	Room* room;

	vec2  position;
	float rotation;

	vector<Geometry*> parts;
	vector<Line*> bumpers;

	void runMotors(int8_t left, int8_t right, uint8_t vac, float dt);
	int isTouchingRoom();
};