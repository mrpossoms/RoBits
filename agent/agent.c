#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <inttypes.h>

#include "agent.h"
#include "RoBitHAL.h"

agent_t* ROBIT_STATE;

int main(int argc, const char* argv[])
{
	if(argc != 2){
		printf("Usage:\n\trobit [shared memory key]\n");
		return 1;
	}

	// attach to the shared memory segment
	key_t uid = (key_t)strtol(argv[1], NULL, 16);
	int shmid = 0;
	if ((shmid = shmget((key_t)uid, sizeof(agent_t), 0666)) < 0) {
		return 1;
	}
	ROBIT_STATE = (agent_t*)shmat(shmid, NULL, 0);
	
	// keep on running
	while(1){
		agentLoop();
		usleep(1000);
	}

	return 0;
}