#ifndef LOGIC_H
#define LOGIC_H

#include"config.h"
#include<stdbool.h>

struct BoardCell{
	int ship_id;
	char status;
	bool shot;
};

struct ShipData{
	int type;
	int health;
	int y,x;
	bool vert;
};

//----functions-

void initialize();
void shoot(int y, int x);
bool placeShip(int ship_index);

//-----externs-
extern int totalHealth;
extern struct ShipData ship[NUM_SHIPS];
extern struct BoardCell board[ROWS][COLS];

#endif
