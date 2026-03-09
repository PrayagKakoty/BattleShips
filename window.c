//Window.c
#include"config.h"
#include"logic.h"
#include<stdio.h>
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

enum {
	TEX_BG,
	TEX_HIT,
	TEX_MISS,
	TEX_B2x1,
	TEX_B4x2,
	TEX_B5x2,
	TEX_LEN
};

const char texture_files[TEX_LEN][FILE_NAME_LEN] = {
	"b2x1.jpg",
	"b4x1.jpg",
	"b5x2.jpg",
	"background.jpg",
	"hit.jpg",
	"miss.jpg",
};
SDL_Texture *textures[TEX_LEN];

//==============================================================>
void gen_board(SDL_Renderer *ren);
SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren);
//---------------------------------------------------------------------------------------------------
void placeShips(SDL_Renderer *ren){
    printf("Entering placeShips()\n");
    SDL_Texture *bg = textures[TEX_BG];
    SDL_Event e;

    int i = 0;
    while(i < NUM_SHIPS){
        // --- Handle all pending events ---
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT) return;
            if(e.type == SDL_MOUSEBUTTONDOWN){
                int x = (e.button.x - BoardOffset_X) / CELL_SIZE;
                int y = (e.button.y - BoardOffset_Y) / CELL_SIZE;

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

        // --- Render start ---
		SDL_SetRenderDrawColor(ren, 0, 100, 255, 255);
		SDL_RenderClear(ren);
        if(bg) SDL_RenderCopy(ren, bg, NULL, NULL);
		//-Rotate-Button-
			SDL_SetRenderDrawColor(ren, 100,70,10,255);
			SDL_Rect rotateButton = {
				RotateButton_X, RotateButton_Y, RotateButtonSize, RotateButtonSize
			};
			SDL_RenderFillRect(ren, &rotateButton);

		//-Grid-
		SDL_SetRenderDrawColor(ren, 255,255,255,255);
		for(int c = 0; c <= COLS; c++)
			SDL_RenderDrawLine(ren, BoardOffset_X + c*CELL_SIZE, BoardOffset_Y, BoardOffset_X + c*CELL_SIZE, BoardOffset_Y + ROWS*CELL_SIZE);
		for(int r = 0; r <= ROWS; r++)
			SDL_RenderDrawLine(ren, BoardOffset_X, BoardOffset_Y + r*CELL_SIZE, BoardOffset_X + COLS*CELL_SIZE, BoardOffset_Y + r*CELL_SIZE);

        //-Ships-Placed-
        SDL_SetRenderDrawColor(ren, 129, 124, 198, 255);
        for(int s = 0; s < i; s++){
			int h,w;
			if(ship[s].vert){
				h = shipType[ship[s].type].height;
				w = shipType[ship[s].type].width;
			}else{
				w = shipType[ship[s].type].height;
				h = shipType[ship[s].type].width;
			}
            SDL_Rect shipRect = {
                BoardOffset_X + ship[s].x*CELL_SIZE,
                BoardOffset_Y + ship[s].y*CELL_SIZE,
                w * CELL_SIZE,
                h * CELL_SIZE
            };
            SDL_RenderFillRect(ren, &shipRect);
        }

        //-Ship-Hover-
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseX >= BoardOffset_X && mouseX < BoardOffset_X+COLS*CELL_SIZE &&
           mouseY >= BoardOffset_Y && mouseY < BoardOffset_Y+ROWS*CELL_SIZE){
            int hoverCol = (mouseX - BoardOffset_X)/CELL_SIZE;
            int hoverRow = (mouseY - BoardOffset_Y)/CELL_SIZE;
			int h,w;
			if(ship[i].vert){
				h = shipType[ship[i].type].height;
				w = shipType[ship[i].type].width;
			}else{
				w = shipType[ship[i].type].height;
				h = shipType[ship[i].type].width;
			}
			
            SDL_Rect hoverRect = {
                BoardOffset_X + hoverCol*CELL_SIZE,
                BoardOffset_Y + hoverRow*CELL_SIZE,
				w * CELL_SIZE,
				h * CELL_SIZE
            };
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); 
            SDL_RenderFillRect(ren, &hoverRect);
        }
        SDL_RenderPresent(ren);
        SDL_Delay(16); // ~60 FPS
    }

    printf("Exiting placeShips()\n");
}

void play(SDL_Renderer *ren){
	printf("Entering play()\n");
	SDL_Texture *bg = textures[TEX_BG];
	int running = 1;
	SDL_Event e;
	while(running){
		//----------------Events--------------------------
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT || totalHealth == 0) running = 0;
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
		//------------------Render-------------------------
		SDL_SetRenderDrawColor(ren, 0, 100,255,255);
		SDL_RenderClear(ren);
		if(bg) SDL_RenderCopy(ren, bg, NULL, NULL);
		gen_board(ren);

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		int hoverCol = -1, hoverRow = -1;
		if(mouseX >= BoardOffset_X && mouseX < BoardOffset_X+COLS*CELL_SIZE && mouseY >= BoardOffset_Y && mouseY < BoardOffset_Y + ROWS*CELL_SIZE){
			hoverCol = (mouseX - BoardOffset_X)/CELL_SIZE;
			hoverRow = (mouseY - BoardOffset_Y)/CELL_SIZE;

			SDL_Rect hoverRect = {
				BoardOffset_X + hoverCol*CELL_SIZE,
				BoardOffset_Y + hoverRow*CELL_SIZE,
				CELL_SIZE, CELL_SIZE
			};
			SDL_SetRenderDrawColor(ren,255,255,255,255);
			SDL_RenderFillRect(ren, &hoverRect);
		}
		SDL_RenderPresent(ren);
	}//End of Render------------------------------

	printf("Exiting play()\n");
	return;
}


//-------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]){
	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL_Init_Error: %s\n", SDL_GetError());
		return 1;
	}
	if(IMG_Init(IMG_INIT_JPG) != IMG_INIT_JPG){
		printf("IMG_Init_Error: %s\n", IMG_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow("Window", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if(!win){
		printf("SDL_Create_Window_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(!ren){
		SDL_DestroyWindow(win);
		printf("SDL_Create_Renderer_Error: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	//-------------------------------------------------------
	
	for(int i = 0; i<TEX_LEN; i++) 
		textures[i] = loadTexture(texture_files[i], ren);

	initialize();
	placeShips(ren);
	play(ren);

	for(int i = 0; i<TEX_LEN; i++)
		SDL_DestroyTexture(textures[i]);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
//============================================================>
void gen_board(SDL_Renderer *ren){
	for(int y = 0; y<ROWS; y++)
	for(int x = 0; x<COLS; x++){
		switch(board[y][x].status){
			case '#': SDL_SetRenderDrawColor(ren, 0,  10, 255, 255); break;
			case 'X': SDL_SetRenderDrawColor(ren, 50, 50, 75, 255); break;
			case '@': SDL_SetRenderDrawColor(ren, 190, 30, 75, 255); break;
			case 'F': SDL_SetRenderDrawColor(ren, 90, 230, 75, 255); break;
		}

		SDL_Rect shipRect = {
			BoardOffset_X + x*CELL_SIZE,
			BoardOffset_Y + y*CELL_SIZE,
			CELL_SIZE,
			CELL_SIZE
		};
		SDL_RenderFillRect(ren, &shipRect);
	}
}
SDL_Texture *loadTexture(const char *file, SDL_Renderer *ren){
	char path[FILE_NAME_LEN+10];
	snprintf(path, sizeof(path), "./assets/%s", file);
	SDL_Surface *temp_Surface = IMG_Load(path);
	if(!temp_Surface){
		printf("IMG_Load_Error: %s\n", IMG_GetError());
		return NULL;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, temp_Surface);
	SDL_FreeSurface(temp_Surface);
	return tex;
}
