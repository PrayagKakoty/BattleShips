//Logic.c
#include"logic.h"
#include<stdbool.h>
#include<stdio.h>

struct BoardCell board[ROWS][COLS];
struct ShipData ship[NUM_SHIPS];

int totalHealth;

void initialize(){
	for(int i = 0; i<ROWS; i++)
	for(int j = 0; j<COLS; j++){
		board[i][j].status = '#';
		board[i][j].shot = false;
		board[i][j].ship_id = -1;
	}
	totalHealth = 0;
	for(int i = 0; i<NUM_SHIPS; i++){
		int id = navy[i];
		ship[i].vert = true;
		ship[i].type = id;
		ship[i].health = shipType[id].height * shipType[id].width;
		totalHealth += ship[i].health;
	}
}

void shoot(int y, int x){
	if(board[y][x].shot == true){
		printf("Already Shot\n");
		return;
	}
	board[y][x].shot = true;
	int ship_id = board[y][x].ship_id;
	if(ship_id == -1){
		board[y][x].status = 'X';
	}else{
		board[y][x].status = '@';
		totalHealth--;
		ship[ship_id].health--;
		if(ship[ship_id].health == 0){
			int h,w;
			if(ship[ship_id].vert){
				h = shipType[ship[ship_id].type].height;
				w = shipType[ship[ship_id].type].width;
			}else{
				w = shipType[ship[ship_id].type].height;
				h = shipType[ship[ship_id].type].width;
			}
			for(int y = ship[ship_id].y; y < h + ship[ship_id].y; y++)
			for(int x = ship[ship_id].x; x < w + ship[ship_id].x; x++){
				board[y][x].status = 'F';
			}
		}
	}
}

bool placeShip(int shipIndex){
	struct ShipData s = ship[shipIndex];
	int l,b;
	if(s.vert){
		l = shipType[s.type].height;
		b = shipType[s.type].width;
	}else{
		b = shipType[s.type].height;
		l = shipType[s.type].width;
	}

	//Out of bound //> or >=?
	if(s.x + b > COLS || s.y + l > ROWS){
		printf("Out of Bounds\n");
		return false;
	}

	//intersect
	for(int i = s.y; i<s.y+l; i++)
	for(int j = s.x; j<s.x+b; j++)
		if(board[i][j].ship_id != -1){
			printf("Intersecting\n");
			return false;
	}

	//place ship if above checks pass
	for(int i = s.y; i<s.y+l; i++)
	for(int j = s.x; j<s.x+b; j++)
		board[i][j].ship_id = shipIndex;
	return true;
}
