#ifndef ROBIT_AGENT
#define ROBIT_AGENT

#include <stdint.h>
#include <stdio.h>

#define MOTOR_LEFT  0
#define MOTOR_RIGHT 1
#define MOTOR_VAC   2

typedef struct{
	uint8_t  buf[256]; // sim
	uint16_t len;      // sim
	uint16_t read;     // agent as data is consumed
} radio_t;

typedef struct{
	uint8_t voltage;     // sim
	uint8_t bumper[6];   // sim
	int16_t odometer[2]; // sim 
	int8_t  motor[3];    // agent
	
	radio_t radio;
} agent_t;

extern agent_t* ROBIT_STATE;

void agentLoop(void);

#endif