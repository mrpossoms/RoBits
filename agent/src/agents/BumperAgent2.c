#include "RoBitHAL.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOW_VOLTAGE 2000
#define IS &
#define DIR2IND(x) (((x) >> 4) - 1)
#define IND2DIR(x) ((x + 1) << 4) 

struct SensorState{
	int bumper;
	int odo[2];
	
};

struct Action{
	int motors[3];
	int duration;	
};

struct StateAction
{
	struct SensorState state;
	struct Action action;
};

uint16_t position[2];
struct StateAction sequence[10];
int sequenceIndex;
int sequenceHistory;

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

int leftDir = ROBIT_WHEEL_FORWARD, rightDir = ROBIT_WHEEL_FORWARD;
int continueTask;

void storeStateAction(int bumper, int odoLeft, int odoRight)
{
	struct StateAction sa = {
		.state = {
			.bumper = bumper,
			.odo = { odoLeft, odoRight },
		},
		.action = {
			.motors = { leftDir, rightDir, 0 },
			.duration = continueTask,
		}
	};

	sequence[sequenceIndex++] = sa;
	sequenceIndex %= 10;

	sequenceHistory++;
	if(sequenceHistory > 10){
		sequenceHistory = 10;
	}
}

void agentLoop(void)
{
	int bumper  = HAL_readBumper();
	int voltage = HAL_readVoltage();

	int odoLeft  = HAL_readEncoder(ROBIT_WHEEL_LEFT, ROBIT_ENC_DELTA);
	int odoRight = HAL_readEncoder(ROBIT_WHEEL_RIGHT, ROBIT_ENC_DELTA);

	int *leftStates, *rightStates;

	if(bumper){
		permissibleStates(&leftStates, &rightStates, bumper);

		printf("sequenceIndex = %d\n", sequenceIndex);

		selectState(leftStates,  &leftDir);
		selectState(rightStates, &rightDir);

		continueTask = 100 + random() % 100;
		storeStateAction(bumper, odoLeft, odoRight);
	}
	else if(!continueTask){
		leftDir = ROBIT_WHEEL_FORWARD;
		rightDir = ROBIT_WHEEL_FORWARD;
	}
	else{
		continueTask--;

		if(!continueTask){
			storeStateAction(bumper, odoLeft, odoRight);
		}
	}

	HAL_driveMotor(ROBIT_WHEEL_LEFT  | leftDir);
	HAL_driveMotor(ROBIT_WHEEL_RIGHT | rightDir);
}