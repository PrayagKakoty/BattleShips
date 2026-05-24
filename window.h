#ifndef WINDOW_H
#define WINDOW_H

#include"config.h"
#include"logic.h"
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

#define BoardOffset_X 150
#define BoardOffset_Y 150
#define CELL_SIZE 40

#define RotateButton_X 10
#define RotateButton_Y 10
#define RotateButtonSize 50

#define InfoBox_X 300
#define InfoBox_Y 10
#define InfoBox_Width 250
#define InfoBox_Height 100

#define FILE_NAME_LEN 20

//--Functions-
void load_textures();
void gen_board();
void get_ship_size(int shipIndex, int *h, int *w);
void place_ships();
void play();
void disp_grid();
void get_ship_rect(int s, int x, int y, SDL_Rect *rect, int *tex);
void disp_hover(int s, int hoverRow, int hoverCol);
void disp_placed_ship(int s);
void render_place_ships(int i);
void render_play();
void exit_game();
#endif
