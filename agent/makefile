CC=gcc
INC=-I ./include -I ./src
SRC=./src/*.c agent.c
AGT_SRC=./src/agents
EXEC=robit

bumper:
	$(CC) $(INC) $(AGT_SRC)/BumperAgent.c $(SRC) -o $(EXEC)

bumper2:
	$(CC) $(INC) $(AGT_SRC)/BumperAgent2.c $(SRC) -o $(EXEC)

all:
	$(CC) $(INC) $(SRC) -o $(EXEC)