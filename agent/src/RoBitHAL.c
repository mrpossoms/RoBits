#include "agent.h"
#include "RoBitHAL.h"
#include <string.h>
#include <assert.h>
#include <math.h>

#define ODO(wheel) ROBIT_STATE->odometer[(wheel)]
#ifndef M_PI
#define M_PI 3.14159265359
#endif

// Sensors
void HAL_positionEstimate(int16_t* coordinate)
{
	coordinate[0] = ROBIT_STATE->position[0] / 0.1;
	coordinate[1] = ROBIT_STATE->position[1] / 0.1;
}

// returns a cardinal direction relative to the local coord sys
int HAL_readBumper(void)
{
	int i = 0;
	i |= (ROBIT_STATE->bumper[0] * 0xFF) & ROBIT_DIR_N;
	i |= (ROBIT_STATE->bumper[1] * 0xFF) & ROBIT_DIR_NW;
	i |= (ROBIT_STATE->bumper[2] * 0xFF) & ROBIT_DIR_SW;
	i |= (ROBIT_STATE->bumper[3] * 0xFF) & ROBIT_DIR_S;
	i |= (ROBIT_STATE->bumper[4] * 0xFF) & ROBIT_DIR_SE;
	i |= (ROBIT_STATE->bumper[5] * 0xFF) & ROBIT_DIR_NE;

	return i;
}

int HAL_readVoltage(void)
{
	return ROBIT_STATE->voltage;
}

// Mechanics
// bit flags with motor and direction or'ed together
int HAL_driveMotor(int flags)
{
	ROBIT_STATE->motor[flags & ROBIT_MOTOR_MSK] = ((flags - ROBIT_WHEEL_STOP) >> 4) * 64;

	return 0;
}

space_t HAL_sample(int16_t pos[2], space_t* point)
{
	// update the min and max bounds of the sample spaces
	if(pos[0] < ROBIT_STATE->spaceMin[0]){
		ROBIT_STATE->spaceMin[0] = pos[0];
		printf("update min[0] to %d\n", pos[0]);
	}
	if(pos[1] < ROBIT_STATE->spaceMin[1]){
		ROBIT_STATE->spaceMin[1] = pos[1];
		printf("update min[1] to %d\n", pos[1]);
	}

	if(pos[0] > ROBIT_STATE->spaceMax[0]){
		ROBIT_STATE->spaceMax[0] = pos[0];
	}
	if(pos[1] > ROBIT_STATE->spaceMax[1]){
		ROBIT_STATE->spaceMax[1] = pos[1];
	}

	int d = pos[0] - (pos[1] * 63);
	int i;

	if(d >= 0){
		i = d % 4000;
	}
	else{
		i = d + 4000 * ceil(-d / 4000.0f);
	}

	// get the old value and set the
	// new one (TODO update with new info)
	space_t ret = ROBIT_STATE->space[i];

	if(point){
		printf("%d\n", i);
		printf("min: (%d, %d)\n", ROBIT_STATE->spaceMin[0], ROBIT_STATE->spaceMin[1]);
		printf("max: (%d, %d)\n", ROBIT_STATE->spaceMax[0], ROBIT_STATE->spaceMax[1]);
		ROBIT_STATE->space[i] = *point;

		assert(ROBIT_STATE->space[i].date == point->date);
		assert(ROBIT_STATE->space[i].isPerimeter == point->isPerimeter);
	}

	return ret;
}


// Communications
static int (*REC_DATA)(struct RobitMessage*);
int HAL_setDataCallback(int (*onData)(struct RobitMessage*))
{
	REC_DATA = onData;

	return 0;
}


int HAL_sendData(struct RobitMessage* data)
{
	ROBIT_STATE->tx.len = sizeof(struct RobitMessage);
	memcpy(ROBIT_STATE->tx.buf, data, ROBIT_STATE->tx.len);
	ROBIT_STATE->tx.ok = 1;

	return 0;
}

void HAL_tick()
{
	// s2m - distance in meters that a wheel transits
	// for 1 / 12 of a rotation
	const float s2m = M_PI * ROBIT_WHEEL_DIA / 12.0f;

	// update the orientation
	float mag = (ODO(ROBIT_WHEEL_RIGHT) + ODO(ROBIT_WHEEL_LEFT)) * s2m;
	ROBIT_STATE->orientation += (ODO(ROBIT_WHEEL_RIGHT) - ODO(ROBIT_WHEEL_LEFT)) * s2m / ROBIT_WHEELBASE;

	// update the position
	ROBIT_STATE->position[0] += mag * sin(ROBIT_STATE->orientation);
	ROBIT_STATE->position[1] += mag * cos(ROBIT_STATE->orientation);

	// reset the odo
	if(ODO(ROBIT_WHEEL_RIGHT) || ODO(ROBIT_WHEEL_LEFT)){
		ODO(ROBIT_WHEEL_RIGHT) = ODO(ROBIT_WHEEL_LEFT) = 0;
	}

	if(ROBIT_STATE->rx.ok){
		REC_DATA((struct RobitMessage*)ROBIT_STATE->rx.buf);
		ROBIT_STATE->rx.ok = 0;
	}
}