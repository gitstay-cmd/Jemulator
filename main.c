//
// Created by jswag on 4/12/18.
//
#include <stdio.h>
#include <stdbool.h>
#include <SDL_events.h>
#include <SDL_timer.h>
#include <SDL.h>

#include "includes.h"
#include "cpu.h"
#include "joypad.h"

CPU *cpu;

void HandleInput(SDL_Event event){
    int key;
    switch(event.type){
            case SDL_KEYUP:
            key = -1;
            switch( event.key.keysym.sym )
            {
                case SDLK_a : key = 4 ; break ;
                case SDLK_s : key = 5 ; break ;
                case SDLK_RETURN : key = 7 ; break ;
                case SDLK_SPACE : key = 6; break ;
                case SDLK_RIGHT : key = 0 ; break ;
                case SDLK_LEFT : key = 1 ; break ;
                case SDLK_UP : key = 2 ; break ;
                case SDLK_DOWN : key = 3 ; break ;
                default: break;
            }
            if (key != -1)
            {
                KeyReleased(cpu->mmu, key);
            }
            break;
        case SDL_KEYDOWN:
            key = -1;
            switch( event.key.keysym.sym )
            {
                case SDLK_a : puts("A down\n");key = 4 ; break ;
                case SDLK_s : puts("S down\n");key = 5 ; break ;
                case SDLK_RETURN : puts("Ret down\n");key = 7 ; break ;
                case SDLK_SPACE :puts("Space down\n"); key = 6; break ;
                case SDLK_RIGHT : puts("Right down\n");key = 0 ; break ;
                case SDLK_LEFT : puts("Left down\n");key = 1 ; break ;
                case SDLK_UP : puts("UP down\n");key = 2 ; break ;
                case SDLK_DOWN :puts("Down\n"); key = 3 ; break ;
                default: break;
            }
            if (key != -1)
            {
                KeyPressed(cpu->mmu, key);
            }
            break;
    }
}

int main(int argc, char *argv[]){

    if(argc < 2) {
        printf("Format: jemulator <rom filename>\n");
        return 0;
    }

	cpu = malloc(sizeof(CPU));
	
    init(cpu, argv[1]);
    bool quit = 0;
    SDL_Event event;

    float fps = 59.73 ;
    float interval = 1000 ;
    interval /= fps;

    FILE *fp = fopen("Dcompile.txt", "w");

    unsigned int time2 = SDL_GetTicks( ) ;

    while (!quit)
    {
        while( SDL_PollEvent( &event ) )
        {
            HandleInput( event ) ;

            if( event.type == SDL_QUIT )
            {
                quit = true;
            }
        }

        unsigned int current = SDL_GetTicks( ) ;

        if( (time2 + interval) < current )
        {
		//	fprintf(fp, "PC: %x\n", cpu->PC.val);
            update(fp, cpu);
            time2 = current ;
//            draw_screen(cpu->mmu->lcd);
//            getchar();
        }

    }
    SDL_Quit( ) ;

    return 0;
}
