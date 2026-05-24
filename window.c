//Window.c
#include"window.h"
#include<stdio.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

enum {
	TEX_BG,
	TEX_ROTATE,
	TEX_HIT,
	TEX_MISS,
	TEX_B2x1V,
	TEX_B4x1V,
	TEX_B3x2V,
	TEX_B2x1H,
	TEX_B4x1H,
	TEX_B3x2H,
	TEX_LEN
};
const char texture_files[TEX_LEN][FILE_NAME_LEN] = {
	"BG.png",
	"rotate.png",
	"HIT.png",
	"MISS.png",
	"B2x1V.png",
	"B4x1V.png",
	"B3x2V.png",
	"B2x1H.png",
	"B4x1H.png",
	"B3x2H.png",
};
SDL_Window *win;
SDL_Texture *textures[TEX_LEN];
SDL_Renderer *ren;

//==============================================================>
void get_ship_rect(int s, int x, int y, SDL_Rect *rect, int *tex){
	int h,w;
	if(ship[s].vert){
		h = shipType[ship[s].type].height;
		w = shipType[ship[s].type].width;
		*tex = TEX_B2x1V + ship[s].type;
	}else{
		w = shipType[ship[s].type].height;
		h = shipType[ship[s].type].width;
		*tex = TEX_B2x1H + ship[s].type;
	}
	*rect = (SDL_Rect){
		x*CELL_SIZE + BoardOffset_X,
		y*CELL_SIZE + BoardOffset_Y,
		w*CELL_SIZE, h*CELL_SIZE
	};
}
void disp_hover(int s, int hoverRow, int hoverCol){
	int tex;
	SDL_Rect rect;
	if(s==-1){
		tex = TEX_HIT;
		rect = (SDL_Rect){
			hoverCol*CELL_SIZE + BoardOffset_X, 
			hoverRow*CELL_SIZE + BoardOffset_Y, 
			CELL_SIZE, CELL_SIZE
		};
	}else{
		get_ship_rect(s, hoverCol, hoverRow, &rect, &tex);
	}
	SDL_RenderCopy(ren, textures[tex], NULL, &rect);
}
void disp_placed_ship(int s){
	int tex;
	SDL_Rect rect;
	get_ship_rect(s, ship[s].x, ship[s].y, &rect, &tex);
	SDL_RenderCopy(ren, textures[tex], NULL, &rect);
}
void render_place_ships(int i){
    SDL_Texture *bg = textures[TEX_BG];
	SDL_SetRenderDrawColor(ren, 0, 100, 255, 255);
	SDL_RenderClear(ren);
	if(bg) SDL_RenderCopy(ren, bg, NULL, NULL);

	//-Rotate-Button-
	SDL_Rect rotateButton = {
		RotateButton_X, RotateButton_Y, RotateButtonSize, RotateButtonSize
	};
	SDL_RenderCopy(ren, textures[TEX_ROTATE], NULL, &rotateButton);

	//-Ships-Placed-
	for(int s = 0; s < i; s++)
		disp_placed_ship(s);

	//-Ship-Hover-
	if(i<NUM_SHIPS){
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		if(mouseX >= BoardOffset_X && mouseX < BoardOffset_X+COLS*CELL_SIZE &&
		   mouseY >= BoardOffset_Y && mouseY < BoardOffset_Y+ROWS*CELL_SIZE){
			int hoverRow = (mouseY - BoardOffset_Y)/CELL_SIZE;
			int hoverCol = (mouseX - BoardOffset_X)/CELL_SIZE;
			disp_hover(i, hoverRow, hoverCol);
		}
	}

	//-Ship-Info-
	if(i<NUM_SHIPS){
		int h,w;
		int tex;
		if(ship[i].vert){
			h = shipType[ship[i].type].height * CELL_SIZE;
			w = shipType[ship[i].type].width * CELL_SIZE;
			tex = TEX_B2x1V + ship[i].type;
		}else{
			w = shipType[ship[i].type].height * CELL_SIZE;
			h = shipType[ship[i].type].width * CELL_SIZE;
			tex = TEX_B2x1H + ship[i].type;
		}

		int x_offset = (InfoBox_Width - w)/2;
		int y_offset = (InfoBox_Height - h)/2;
		SDL_Rect infoBox = {
			InfoBox_X + x_offset, InfoBox_Y+y_offset, w, h
		};
		SDL_RenderCopy(ren, textures[tex], NULL, &infoBox);
	}

	disp_grid();
	SDL_RenderPresent(ren);
}
void render_play(){
    SDL_Texture *bg = textures[TEX_BG];
	SDL_SetRenderDrawColor(ren, 0, 100,255,255);
	SDL_RenderClear(ren);
	if(bg) SDL_RenderCopy(ren, bg, NULL, NULL);
	gen_board();

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	if(mouseX >= BoardOffset_X && mouseX < BoardOffset_X + COLS*CELL_SIZE 
	&& mouseY >= BoardOffset_Y && mouseY < BoardOffset_Y + ROWS*CELL_SIZE){
		int hoverCol = (mouseX - BoardOffset_X)/CELL_SIZE;
		int hoverRow = (mouseY - BoardOffset_Y)/CELL_SIZE;
		disp_hover(-1, hoverRow, hoverCol);
	}
	SDL_RenderPresent(ren);
}

void place_ships(){
    printf("Entering place_ships()\n");
    SDL_Event e;

    int i = 0;
    while(i < NUM_SHIPS){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) exit_game();
            if(e.type == SDL_MOUSEBUTTONDOWN){
				if(e.button.x < BoardOffset_X || e.button.x >= (BoardOffset_X+COLS*CELL_SIZE) 
				|| e.button.y < BoardOffset_Y || e.button.y >= (BoardOffset_Y + ROWS*CELL_SIZE) ){
					if(e.button.x > RotateButton_X && e.button.x <= (RotateButton_X + RotateButtonSize) 
					&& e.button.y > RotateButton_Y && e.button.y <= (RotateButton_Y + RotateButtonSize) ){
						printf("Rotate\n");
						ship[i].vert = !ship[i].vert;
					}
					else{
						printf("Out of Board\n");
					}
					continue;
				}

                int x = (e.button.x - BoardOffset_X) / CELL_SIZE;
                int y = (e.button.y - BoardOffset_Y) / CELL_SIZE;
                ship[i].x = x;
                ship[i].y = y;
                if(placeShip(i)){
                    printf("Placed Ship %d at %d,%d\n", i, y, x);
                    i++; // only increment when placement succeeds
                } else {
                    printf("Cannot place ship here\n");
                }
            }
        }
		render_place_ships(i);
    }

    printf("Exiting place_ships()\n");
}

void play(){
	printf("Entering play()\n");
	int running = 1;
	SDL_Event e;
	while(running){
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT) exit_game();
			if(totalHealth == 0) running = 0;
			if(e.type == SDL_MOUSEBUTTONDOWN){
				if(e.button.x < BoardOffset_X || e.button.x >= (BoardOffset_X+COLS*CELL_SIZE) || e.button.y < BoardOffset_Y || e.button.y >= BoardOffset_Y + ROWS*CELL_SIZE){
					printf("Out of Bounds\n");
				}else{
					int x = (e.button.x-BoardOffset_X) / CELL_SIZE;
					int y = (e.button.y-BoardOffset_Y) / CELL_SIZE;
					shoot(y,x);
					printf("Clicked Cell: %d %d\n", y, x);
				}
			}
		}
		render_play();
	}
	printf("Exiting play()\n");
	return;
}

//-------------------------------------------------------------------------------------------------

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init_Error: %s\n", SDL_GetError());
		return 1;
	}
	if(IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG){
		printf("IMG_Init_Error: %s\n", IMG_GetError());
		SDL_Quit();
		return 1;
	}
	win = SDL_CreateWindow("Window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(!win){
		printf("SDL_Create_Window_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!ren){
		SDL_DestroyWindow(win);
		printf("SDL_Create_Renderer_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//-------------------------------------------------------
	
	load_textures();

	initialize();
	place_ships();
	play();

	exit_game();
	return 0;
}
//============================================================>
void gen_board(){
	disp_grid();
	for(int y = 0; y<ROWS; y++)
	for(int x = 0; x<COLS; x++){
		SDL_Rect rect = {
			BoardOffset_X + x*CELL_SIZE,
			BoardOffset_Y + y*CELL_SIZE,
			CELL_SIZE,
			CELL_SIZE
		};
		switch(board[y][x].status){
			case '@': SDL_RenderCopy(ren, textures[TEX_HIT], NULL, &rect); break;
			case 'X': SDL_RenderCopy(ren, textures[TEX_MISS], NULL, &rect); break;
		}
	}
	for(int s = 0; s<NUM_SHIPS; s++){
		if(ship[s].health == 0){
			int tex;
			SDL_Rect rect;
			get_ship_rect(s,ship[s].x,ship[s].y, &rect, &tex);
			SDL_RenderCopy(ren, textures[tex], NULL, &rect);
		}
	}
}
void load_textures(){
	for(int i = 0; i<TEX_LEN; i++) {
		char path[FILE_NAME_LEN+10];
		snprintf(path, sizeof(path), "./assets/%s", texture_files[i]);
		SDL_Surface *temp_Surface = IMG_Load(path);
		if(!temp_Surface){
			printf("IMG_Load_Error: %s\n", IMG_GetError());
			continue;
		}

		textures[i] = SDL_CreateTextureFromSurface(ren, temp_Surface);
		SDL_FreeSurface(temp_Surface);
	}
}
void disp_grid(){
	SDL_SetRenderDrawColor(ren, 255,255,255,255);
	for(int c = 0; c <= COLS; c++){
		SDL_RenderDrawLine(ren, BoardOffset_X + c*CELL_SIZE, BoardOffset_Y, BoardOffset_X + c*CELL_SIZE, BoardOffset_Y + ROWS*CELL_SIZE);
	}
	for(int r = 0; r <= ROWS; r++){
		SDL_RenderDrawLine(ren, BoardOffset_X, BoardOffset_Y + r*CELL_SIZE, BoardOffset_X + COLS*CELL_SIZE, BoardOffset_Y + r*CELL_SIZE);
	}
}
void exit_game(){
	for(int i = 0; i<TEX_LEN; i++)
		SDL_DestroyTexture(textures[i]);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	exit(1);
}
