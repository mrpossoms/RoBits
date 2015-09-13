#ifndef ROBIT_AGENT
#define ROBIT_AGENT

#include <stdint.h>
#include <stdio.h>

#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1
#define MOTOR_VAC   2


typedef struct{
	uint8_t  buf[512]; // sim
	uint16_t len;      // sim,
	uint16_t ok;
} radio_t;

typedef struct{
	uint8_t date;
	uint8_t isPerimeter;
} space_t;

typedef struct{
	float orientation; // 4
	float position[2]; // 8

	uint8_t voltage;     // sim 1
	uint8_t bumper[6];   // sim 6
	int16_t odometer[2]; // sim 4
	int8_t  motor[3];    // agent 3
	
	int16_t spaceMin[2], spaceMax[2]; // 8
	space_t space[4000]; // 8000

	radio_t rx, tx; // 2 * 516 (1032) 
} agent_t;

extern agent_t* ROBIT_STATE;

void agentSetup(void);
void agentLoop(void);

#endif