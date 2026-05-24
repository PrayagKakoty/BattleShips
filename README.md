 To compile this :
     gcc window.c logic.c config.c $(sdl2-config --cflags --libs) -lSDL2_image


window.c:
    load_textures();
    logic.initialize();

    place_ships(){
        for(i in NUM_SHIPS){
            if(mouse outside board_bounds && inside rotate_button)
                rotate ship;
            else{
                x,y = (mouse.x,y - BoardOffset_X,Y) / CELL_SIZE;
                ship[i].pos = (x,y)
                try(logic.placeShip(i))
                    i++;
                else printf("Cannot place here")
            }
            render_place_ship(i);
        }
    }

    render_place_ships(i){ //ith ship
        SDL_RenderCopy(background);
        SDL_RenderCopy(rotate_button);
        
        for(s = 0 to i)
            disp_placed_ship(s);

        SDL_GetMouseState(&mouseX, &mouseY)
        if(mouse within board_bounds)
            disp_hover(i, row, col);

        disp_grid();
    }
    play();
    exit_game();
