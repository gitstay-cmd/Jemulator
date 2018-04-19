#ifndef lcd_h_INCLUDED
#define lcd_h_INCLUDED
#include "includes.h"

#include <SDL_video.h>
#include <SDL.h>

#include <GL/glew.h>

#include <stdint.h>
#include <stdio.h>

#define WHITE 0x00
#define LIGHT_GRAY 0x01
#define DARK_GRAY 0x10
#define BLACK 0x11

#define windowWidth 160
#define windowHeight 144

typedef struct color_S{
    union {
        struct{
        	BYTE blue;
    		BYTE green;
    		BYTE red;
    		BYTE alpha;
        };
        int color;
    };
}COLOR;

typedef struct lcd_S {
    BYTE status;	//Status 0xFF41
    BYTE sc_l; 		//Current scanline 0xFF44 
    int sl_c;		//Scanline counter 
    BYTE sl_g;		//Scanline game is interested in 0xFF45
	BYTE enabled;
	BYTE scrolly;	//Scroll Y 0xFF42
	BYTE scrollx;	//Scroll X 0xFF43
	BYTE windowx;	//Window X 0xFF4A
	BYTE windowy;	//Window X 0xFF4B
	BYTE screen[windowWidth][windowHeight][3];
	SDL_Window *window;
	union { //Control register 0xFF40 
    	struct {
        	BYTE bg_d: 1; //BG Display
        	BYTE s_disp_en: 1; //Sprite Display Enable
        	BYTE size: 1; //Sprite Size(0=8x8, 1=8x16)
        	BYTE tm_disp: 1; //BG Tile Map Display Select(0=9800-9BFF, 1=9C00-9FFF)
        	BYTE td_sel: 1; //BG & Window Tile Data Select(0=8800-97FF, 1=8000-8FFF)
        	BYTE w_disp_en: 1; //Window Display Enable
        	BYTE w_tm_disp: 1; //Window Tile Map Display Select(0=9800-9BFF, 1=9C00-9FFF)
        	BYTE en: 1; //LCD Display Enable
    	};
    	BYTE ctl_r;
	};
}LCD;

typedef struct sprite_S {
	BYTE spriteY;
	BYTE spriteX;
	BYTE pattern;
	union {
		struct{
			BYTE notused2: 3;
			BYTE notused1: 1;
			BYTE pallete: 1;
			BYTE xflip: 1;
			BYTE yflip: 1;
			BYTE priority: 1;
		};
		BYTE attributes;
	};
}SPRITE;

void UpdateGraphics(void *mmu, int cycles);
BYTE IsLCDEnabled(LCD *lcd);
void DrawScanLine(void *mmu);
void Render_Screen(FILE *fp, LCD *lcd);
int init_screen(LCD *lcd);
void draw_screen(LCD *lcd);
#endif // lcd_h_INCLUDED

