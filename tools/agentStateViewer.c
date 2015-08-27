#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include "agent.h"

agent_t* S;

void line(int x, int y, const char* str)
{
	int i, len = strlen(str);
	int dx = x;

	for(i = 0; i < len; i++){
		if(str[i] == '\n'){
			++y;
			dx = x;
		}
		mvaddch(y, dx++, str[i]);
	}
}

int main(int argc, const char* argv[]){
	initscr();
	clear();

	key_t uid = (key_t)strtol(argv[1], NULL, 16);

	int shmid = 0;
	if ((shmid = shmget((key_t)uid, sizeof(agent_t), 0666)) < 0) {
		return 1;
	}
	S = (agent_t*)shmat(shmid, NULL, 0);

	while(1){
		char buf[256];

		clear();

		sprintf(buf, "BUMPERS [ %d, %d, %d, %d, %d, %d ]\nODO (%d, %d)",
			S->bumper[0], S->bumper[1], S->bumper[2],
			S->bumper[3], S->bumper[4], S->bumper[5],
			S->odometer[MOTOR_LEFT], S->odometer[MOTOR_RIGHT]
		);
		line(1, 1, buf);
		refresh();

		usleep(10000);
	}

	return 0;
}
