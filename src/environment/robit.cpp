#include "robit.h"

#include "geometry/line.h"
#include "geometry/rectangle.h"
#include "geometry/circle.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define ROBIT_RAD 0.1

static float WHEEL_BASE;

void Robit::createParts()
{
	parts.push_back(new Circle(VEC2_ZERO, 0.85 * ROBIT_RAD, 0xFF00FFFF));

	vec2 ull = { -0.6 * ROBIT_RAD, -0.15 * ROBIT_RAD};
	vec2 lrl = { -0.7 * ROBIT_RAD,  0.15 * ROBIT_RAD};
	parts.push_back(new Rectangle(ull, lrl, 0xFF00FFFF));

	vec2 ulr = { 0.6 * ROBIT_RAD, -0.15 * ROBIT_RAD};
	vec2 lrr = { 0.7 * ROBIT_RAD,  0.15 * ROBIT_RAD};
	parts.push_back(new Rectangle(ulr, lrr, 0xFF00FFFF));

	float dt = M_PI / 3.0;
	float off = M_PI / 6;
	for(int i = 6; i--;){
		vec2 p1 = { sin((i+0) * dt + 0.2 - off) * ROBIT_RAD, cos((i+0) * dt + 0.2 - off) * ROBIT_RAD };
		vec2 p2 = { sin((i+1) * dt - 0.2 - off) * ROBIT_RAD, cos((i+1) * dt - 0.2 - off) * ROBIT_RAD };

		Line* bumper = new Line(p1, p2, 0xFF00FFFF);

		bumpers.push_back(bumper);
	}	
}

agent_t* Robit::attachShm(uint32_t key)
{
	int shmid = 0;
	errno = 0;
	if ((shmid = shmget((key_t)key, sizeof(agent_t), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
	}

	if (errno) {
        perror("shmat");
        assert(!errno);
    }

	errno = 0;

	return (agent_t*)shmat(shmid, NULL, 0);
}

Robit::Robit(Room* r, uint32_t uid)
{
	createParts();
	state = attachShm(uid);

	bzero(state, sizeof(agent_t));

	id = uid;
	room = r;
	rotation = RAND_F * M_PI * 2;
	position[0] = 1;
	position[1] = -0.5;
	// vec2_set(position, VEC2_ZERO);
}

int Robit::runMotors(int8_t left, int8_t right, uint8_t vac, float dt)
{
	const float radius = 0.65 * ROBIT_RAD;
	vec2 oPrime, oTemp;
	vec2 lastPosition;
	float lastRotation;

	vec2_set(lastPosition, position);
	lastRotation = rotation;

	left  = left > 0 ? 64 : (left < 0 ? -64 : left);
	right = right > 0 ? 64 : (right < 0 ? -64 : right);

	// cir = 2πr
	// arclen = θr
	// θ = arclen / r
	// v' = |v-o| * (cos(θ), sin(θ)) + o # where o is the origin of rotation
	{
		vec2 leftWheel  = { cosf(rotation) * -radius, sinf(rotation) * -radius };

		float lTheta = dt * (left / 2550.0f) / radius;
		float c = cosf(lTheta), s = sinf(lTheta);
		vec2_add(leftWheel, leftWheel, position);
		vec2_sub(oPrime, position, leftWheel);

		oTemp[0] = c * oPrime[0] - s * oPrime[1];
		oTemp[1] = s * oPrime[0] + c * oPrime[1];

		vec2_add(position, oTemp, leftWheel);
		rotation += lTheta; 
	}

	{
		vec2 rightWheel  = { cosf(rotation) * radius, sinf(rotation) * radius };

		float rTheta = dt * (-right / 2550.0f) / radius;
		float c = cosf(rTheta), s = sinf(rTheta);
		vec2_add(rightWheel, rightWheel, position);
		vec2_sub(oPrime, position, rightWheel);

		oPrime[0] = c * oPrime[0] - s * oPrime[1];
		oPrime[1] = s * oPrime[0] + c * oPrime[1];

		vec2_add(position, oPrime, rightWheel);
		rotation += rTheta;
	}

	if(isTouchingRoom()){
		vec2_set(position, lastPosition);
		rotation = lastRotation;

		return 1;
	}
	else{
		state->odometer[MOTOR_LEFT]  += left;
		state->odometer[MOTOR_RIGHT] += right;
	}

	return 0;
}

int Robit::isTouchingRoom()
{
	float t = 0;
	for(int i = room->perimeter.size(); i--;){
		if(this->intersects(room->perimeter[i], NULL, &t) > 0){
			return 1;
		}
	}

	return 0;
}

int Robit::intersects(Geometry* geo, vec2 normal, float* t)
{
	// Set all the bumpers to not colliding
	bzero(state->bumper, sizeof(state->bumper));

	for(int i = bumpers.size(); i--;){
		Line* bumper = bumpers[i];
		Line transformed;

		mat4x4 model, temp;

		mat4x4_identity(temp);
		mat4x4_translate_in_place(temp, position[0], position[1], 0);
		mat4x4_rotate_Z(model, temp, rotation);

		mat4x4_mul_vec4(transformed.vertices[0], model, bumper->vertices[0]);
		mat4x4_mul_vec4(transformed.vertices[1], model, bumper->vertices[1]);

		float t = 0;

		if(transformed.intersects(geo, normal, &t)){
			state->bumper[i] = 1;
			bumper->setColor(0xFFFFFFFF);
			return 1;
		}
	}

	// vec2 norm;
	// Circle c(position, ((Circle*)parts[0])->radius, 0);
	// return c.intersects(geo, norm);

	return 0;
}

void Robit::update(float dt)
{
	for(int i = bumpers.size(); i--;){
		bumpers[i]->setColor(0xFF0000FF);
	}

	for(int i = 100; i--;){
		if(runMotors(
			state->motor[MOTOR_LEFT],
			state->motor[MOTOR_RIGHT],
			state->motor[MOTOR_VAC],
			dt / 100.0
		)){
			break;
		}
	}

	bzero(state->motor, sizeof(state->motor));
}

void Robit::draw(mat4x4 viewProjection)
{
	mat4x4 model, temp;

	mat4x4_identity(temp);
	mat4x4_translate_in_place(temp, position[0], position[1], 0);
	mat4x4_rotate_Z(model, temp, rotation);

	mat4x4_mul(temp, viewProjection, model);

	for(int i = parts.size(); i--;){
		parts[i]->draw(temp);
	}

	glLineWidth(3.0);
	glBegin(GL_LINES);

	for(int i = bumpers.size(); i--;){
		bumpers[i]->draw(temp);
	}

	glEnd();
}