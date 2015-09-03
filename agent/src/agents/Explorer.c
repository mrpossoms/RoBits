#include "RoBitHAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOW_VOLTAGE 2000
#define IS &
#define DIR2IND(x) (((x) >> 4) - 1)
#define IND2DIR(x) ((x + 1) << 4) 

void permissibleStates(int** left, int** right, int bumper)
{
	const int normal[3] = { 1, 0, 1 };

	// don't allow standing still by default
	static int allowedLeft[3];
	static int allowedRight[3];

	memcpy(allowedLeft,  normal, sizeof(normal));
	memcpy(allowedRight, normal, sizeof(normal));

	// prune the permitted movement states
	// based on the bumper readings
	if(bumper IS ROBIT_DIR_N){
		allowedLeft [DIR2IND(ROBIT_WHEEL_FORWARD)] = 0;
		allowedRight[DIR2IND(ROBIT_WHEEL_FORWARD)] = 0;
	}
	if(bumper IS ROBIT_DIR_NW){
		allowedLeft[DIR2IND(ROBIT_WHEEL_FORWARD)] = 0;
	}
	if(bumper IS ROBIT_DIR_SW){
		allowedLeft[DIR2IND(ROBIT_WHEEL_BACKWARD)] = 0;	
	}
	if(bumper IS ROBIT_DIR_S){
		allowedLeft [DIR2IND(ROBIT_WHEEL_BACKWARD)] = 0;
		allowedRight[DIR2IND(ROBIT_WHEEL_BACKWARD)] = 0;		
	}
	if(bumper IS ROBIT_DIR_SE){
		allowedRight[DIR2IND(ROBIT_WHEEL_BACKWARD)] = 0;			
	}
	if(bumper IS ROBIT_DIR_NE){
		allowedRight[DIR2IND(ROBIT_WHEEL_FORWARD)] = 0;		
	}

	*left  = allowedLeft;
	*right = allowedRight;
}

void selectState(int* states, int* dir){
	int ri = random() % 3;
	for(int i = 3; i--;){
		if(states[i]){
			*dir = IND2DIR((i + ri) % 3);
			break;
		}
	}	
}

void agentLoop(void)
{
	uint16_t pos[2];

	HAL_positionEstimate(pos);


	HAL_driveMotor(ROBIT_WHEEL_LEFT  | ROBIT_WHEEL_FORWARD);
	HAL_driveMotor(ROBIT_WHEEL_RIGHT | ROBIT_WHEEL_FORWARD);
}