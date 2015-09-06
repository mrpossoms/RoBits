#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "agent.h"
#include "RoBitHAL.h"

agent_t* ROBIT_STATE;

space_t mark(int16_t pos[2], space_t* point)
{

	int d = pos[0] - (pos[1] * 63);
	int i = d - (4000 * (int)(d / 4000.0f));

	// get the old value and set the
	// new one (TODO update with new info)
	space_t ret = ROBIT_STATE->space[i];

	if(point){
		ROBIT_STATE->space[i] = *point;		
	}

	return ret;
}

int main(int argc, const char* argv[])
{
	if(argc != 2){
		printf("Usage:\n\trobit [shared memory key]\n");
		return 1;
	}

	// attach to the shared memory segment
	key_t uid = (key_t)strtoll(argv[1], NULL, 16);
	int shmid = 0;
	if ((shmid = shmget((key_t)uid, sizeof(agent_t), 0666)) < 0) {
		printf("0x%x\n", uid);
		printf("Shared memory error (%d)\n", errno);
		return 1;
	}
	ROBIT_STATE = (agent_t*)shmat(shmid, NULL, 0);
	
	struct timespec delay = {
		0, 10000
	};

	// keep on running
	while(1){
		HAL_tick();
		agentLoop();
		nanosleep(&delay, NULL);
	}

	return 0;
}