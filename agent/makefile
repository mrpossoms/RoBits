CC=gcc
CFLAGS=-std=c99 -D_SVID_SOURCE -D_POSIX_C_SOURCE=199309L
INC=-I ./include -I ./src
SRC=./src/*.c agent.c
AGT_SRC=./src/agents
EXEC=robit
LINK=-lm

bumper:
	$(CC) $(CFLAGS) $(INC) $(AGT_SRC)/BumperAgent.c $(SRC) -o $(EXEC) $(LINK)

bumper2:
	$(CC) $(CFLAGS) $(INC) $(AGT_SRC)/BumperAgent2.c $(SRC) -o $(EXEC) $(LINK)

explorer:
	$(CC) $(CFLAGS) $(INC) $(AGT_SRC)/Explorer.c $(SRC) -o $(EXEC) $(LINK)

all:
	$(CC) $(CFLAGS) $(INC) $(SRC) -o $(EXEC) $(LINK)