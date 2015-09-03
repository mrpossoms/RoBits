#include "agent.h"
#include "RoBitHAL.h"
#include <string.h>
#include <math.h>

static float orientation;
static float position[2];

#define ODO(wheel) ROBIT_STATE->odometer[(wheel)]

// Sensors
void HAL_positionEstimate(int16_t* coordinate)
{
	coordinate[0] = position[0] / 0.1;
	coordinate[1] = position[1] / 0.1;
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
	const float s2m = ROBIT_WHEEL_DIA / 12.0f;

	// update the orientation
	float mag = (ODO(ROBIT_WHEEL_RIGHT) + ODO(ROBIT_WHEEL_LEFT)) * s2m / 2;
	orientation += (ODO(ROBIT_WHEEL_RIGHT) - ODO(ROBIT_WHEEL_LEFT)) * s2m / ROBIT_WHEELBASE;

	// update the position
	position[0] += mag * sin(orientation);
	position[1] += mag * cos(orientation);

	// reset the odo
	if(ODO(ROBIT_WHEEL_RIGHT) || ODO(ROBIT_WHEEL_LEFT)){
		ODO(ROBIT_WHEEL_RIGHT) = ODO(ROBIT_WHEEL_LEFT) = 0;
	}

	if(ROBIT_STATE->rx.ok){
		REC_DATA((struct RobitMessage*)ROBIT_STATE->rx.buf);
		ROBIT_STATE->rx.ok = 0;
	}
}