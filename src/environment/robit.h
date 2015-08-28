#pragma once

#include "geometry.h"
#include "geometry/line.h"
#include "room.h"
#include <vector>

#include "agent.h"

using namespace std;

class Robit{
public:
	Robit(Room* room, uint32_t id);
	~Robit();

	int intersects(Geometry* geo, vec2 normal, float* t);
	void update(float dt);
	void draw(mat4x4 viewProjection);

	vec2  position;
private:
	Room* room;

	float rotation;

	vector<Geometry*> parts;
	vector<Line*> bumpers;

	uint32_t id;

	agent_t* state;

	void createParts(void);
	agent_t* attachShm(uint32_t key);

	int runMotors(int8_t left, int8_t right, uint8_t vac, float dt);
	int isTouchingRoom();
};