#include "RoBitHAL.h"
#include <stdio.h>
#include <stdlib.h>

int backup = 0;
int turn = 0;
int left = ROBIT_WHEEL_STOP, right = ROBIT_WHEEL_STOP;

void agentLoop(void)
{
	if(HAL_readBumper()){
		backup = 200;
	}

	if(backup){
		backup--;
		turn = random() % 100 + 100;

		left  = ROBIT_WHEEL_BACKWARD;
		right = ROBIT_WHEEL_BACKWARD;

		if(!backup){
			if(random() % 2){
				left  = ROBIT_WHEEL_FORWARD;
				right = ROBIT_WHEEL_BACKWARD;
			}
			else{
				left  = ROBIT_WHEEL_BACKWARD;
				right = ROBIT_WHEEL_FORWARD;
			}
		}
	}
	else if(turn){
		turn--;
	}
	else{
		left  = ROBIT_WHEEL_FORWARD;
		right = ROBIT_WHEEL_FORWARD;
	}

	HAL_driveMotor(ROBIT_WHEEL_LEFT  | left);
	HAL_driveMotor(ROBIT_WHEEL_RIGHT | right);
}