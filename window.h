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

#define FILE_NAME_LEN 20

//--Functions-
void gen_board(SDL_Renderer *ren);
SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren);
void get_ship_size(int shipIndex, int *h, int *w);
void place_ships(SDL_Renderer *ren);
void play(SDL_Renderer *ren);
void disp_grid(SDL_Renderer *ren);
#endif
