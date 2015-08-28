#ifndef ROBIT_HAL
#define ROBIT_HAL

// Hardware
#define ROBIT_MOTOR_MSK   0x0F
#define ROBIT_WHEEL_LEFT  0x00
#define ROBIT_WHEEL_RIGHT 0x01
#define ROBIT_VACUUME     0x02

#define ROBIT_WHEEL_FORWARD   0x30
#define ROBIT_WHEEL_STOP      0x20
#define ROBIT_WHEEL_BACKWARD  0x10

// read the encoder difference since last read
#define ROBIT_ENC_DELTA 0x01

// Spatial
#define ROBIT_DIR_N  0x01
#define ROBIT_DIR_NW 0x02
#define ROBIT_DIR_SW 0x04
#define ROBIT_DIR_S  0x10
#define ROBIT_DIR_SE 0x20
#define ROBIT_DIR_NE 0x40

// Types
typedef unsigned char byte;

// Data & structs
struct RobitMessage{
	byte botId;     // unique identifier for the bot that sent this message
	byte len;       // bytes that will follow this one
	byte type;      
	byte data[256]; // generic data packet
};

// Sensors
int HAL_readEncoder(int wheel, int flags);
int HAL_readBumper(void); // returns a cardinal direction relative to the local coord sys
int HAL_readVoltage(void);

// Mechanics
int HAL_driveMotor(int flags); // bit flags with motor and direction or'ed together

// Communications
int HAL_setDataCallback(int (*onData)(struct RobitMessage*));
int HAL_sendData(struct RobitMessage* data);

void HAL_tick();

#endif