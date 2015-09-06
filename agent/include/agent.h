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
	uint8_t voltage;     // sim
	uint8_t bumper[6];   // sim
	int16_t odometer[2]; // sim 
	int8_t  motor[3];    // agent
	
	int16_t spaceMin[2], spaceMax[2];
	space_t space[4000];

	radio_t rx, tx;
} agent_t;

extern agent_t* ROBIT_STATE;

void agentLoop(void);
space_t mark(int16_t pos[2], space_t* point);

#endif