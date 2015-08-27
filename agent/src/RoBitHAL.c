#include "agent.h"
#include "RoBitHAL.h"

// Sensors
int HAL_readEncoder(int wheel, int flags)
{
	int reading = ROBIT_STATE->odometer[wheel];

	// reset to get the next delta
	if(flags & ROBIT_ENC_DELTA){
		ROBIT_STATE->odometer[wheel] = 0;
	}

	return reading;
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
int HAL_setDataCallback(int (*onData)(struct RobitMessage*))
{
	return 0;
}


int HAL_sendData(struct RobitMessage* data)
{
	return 0;
}
