#ifndef CONFIG_H
#define CONFIG_H

#define ROWS 10
#define COLS 10

#define NUM_SHIPS 4
#define NUM_SHIP_TYPES 3
#define SHIP_NAME_LEN 10

struct ShipType{
	char name[SHIP_NAME_LEN];
	int height, width;
};

extern struct ShipType shipType[NUM_SHIP_TYPES];
extern int navy[NUM_SHIPS];

#endif
