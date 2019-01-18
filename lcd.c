#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "lcd.h"
#include "mmu.h"
#include "interrupts.h"

SDL_Window *window = NULL;
SDL_Surface  *screenSurface = NULL;

void draw_screen(LCD *lcd){
    for(int i = 0; i < windowWidth; i++){
        for(int j = 0; j < windowHeight; j++){
            switch(lcd->screen[i][j][0]){
                case 255:
                    printf("W ");
                    break;
                case 0xCC:
                    printf("LG ");
                    break;
                case 0x77:
                    printf("DG ");
                    break;
                default:
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
    printf("\n");
}


int reset_screen(LCD *lcd){
    memset(lcd->screen,0xFF, 3 * windowWidth * windowHeight);
    return 0;
}

int init_screen(LCD *lcd) {
    memset(lcd->screen, 0xFF, 3* windowWidth * windowHeight);
    lcd->status = 0;
    lcd->sc_l = 0;
    lcd->sl_c = 0;
    lcd->sl_g = 0;
    lcd->enabled = 0;
    lcd->scrolly = 0;
    lcd->scrollx = 0;
    lcd->windowx = 0;
    lcd->windowy = 0;
    lcd->ctl_r = 0x91;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return 0;
    }
   	window = SDL_CreateWindow("My Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowWidth, windowHeight, SDL_WINDOW_SHOWN);

	if(!window){
    	return 0;
	}

	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0x00, 0x00));

	SDL_UpdateWindowSurface(window);	

	return 1;
}

void Render_Screen(FILE *fp, LCD *lcd){
    
	int *pixels = (int*)screenSurface->pixels;	

	for(int x = 0; x < windowWidth; x++){
        for(int y = 0; y < windowHeight; y++){
            pixels[x + (y * windowWidth)] |= 0xff000000;
            pixels[x + (y * windowWidth)] |= lcd->screen[x][y][0] << 16;
            pixels[x + (y * windowWidth)] |= lcd->screen[x][y][1] << 8;
            pixels[x + (y * windowWidth)] |= lcd->screen[x][y][2];
        }
    }
    SDL_UpdateWindowSurface(window);
}

void SetLCDStatus(LCD *lcd){
    if(!lcd->en){
        lcd->sl_c = 456;
        lcd->status &= 252;
        lcd->status |= 1;
        lcd->sc_l = 0;
        return;
    }

    BYTE currentline = lcd->sc_l;
    BYTE currentmode = (BYTE)(lcd->status & 0x3);

    BYTE mode = 0;
    BYTE reqInt = 0;

    if( currentline >= 144){
        mode = 1;
        lcd->status |= 1;
        lcd->status &= ~0x2;
        reqInt = (BYTE)(lcd->status & 0x10)? 1: 0;
    }else{
        int mode2bounds = 456-80;
        int mode3bounds = mode2bounds - 172;

        //mode 2
        if(lcd->sl_c >= mode2bounds){
            mode = 2;
            lcd->status |= 0x2;
            lcd->status &= ~0x1;
            reqInt = (BYTE)(lcd->status & (0x20))? 1: 0;

        }

            //mode 3
        else if(lcd->sl_c >= mode3bounds){
            mode = 3;
            lcd->status |= 0x3;
        }
            //mode 0
        else{
            mode = 0;
            lcd->status &= ~0x3;
            reqInt = (BYTE)(lcd->status & 0x8)? 1: 0;
        }
    }

    if(reqInt && (mode != currentmode))
        RequestInterrupt(1);

    if(lcd->sc_l == lcd->sl_g){
        lcd->status |=0x4;
        if(lcd->status & (1 << 6)){
            RequestInterrupt(1);
        }
    }
    else{
        lcd->status &= ~0x4;
    }
}

void UpdateGraphics(void *mmu, int cycles){
    LCD *lcd = ((MMU*)mmu)->lcd;
    SetLCDStatus(lcd);
	if(IsLCDEnabled(lcd)){
    	lcd->sl_c -= cycles;
    	
	}else {
    	return;
	}
	if (lcd->sl_c <= 0){
    	lcd->sc_l++;
    	lcd->sl_c = 456;

    	if(lcd->sc_l == 144){
        	RequestInterrupt(0);
        	
    	}else if (lcd->sc_l > 153){
        	lcd->sc_l = 0;
    	}else if (lcd->sc_l < 144){
        	DrawScanLine(mmu);
    	}
	}
}

BYTE GetColour(MMU *mmu, BYTE colourNum, SHORT address){
    BYTE res = WHITE;
    BYTE palette = read_byte(mmu, address);
    int hi = 0;
    int lo = 0;
    switch(colourNum){
        case 0: hi = 1; lo = 0; break;
        case 1: hi = 3; lo = 2; break;
        case 2: hi = 5; lo = 4; break;
        case 3: hi = 7; lo = 6; break;
        default: break;
    }

    int colour = 0;
    colour = (palette & (1 << hi))? 1: 0;
    colour <<= 1;
    colour |= (palette & (1 << lo))? 1: 0;

    switch(colour){
        case 0: res = WHITE; break;
        case 1: res = LIGHT_GRAY; break;
        case 2: res = DARK_GRAY; break;
        case 3: res = BLACK; break;
        default: break;
    }

    return res;
}

BYTE IsLCDEnabled(LCD *lcd){
    return (BYTE)(lcd->en);
}

void RenderTiles(MMU *mmu){

    LCD *lcd = mmu->lcd;
    SHORT tileData = 0;
    SHORT backgroundMem = 0;
    BYTE unsig = 1;

    BYTE useWind = 0;

    if(lcd->w_disp_en) {
        if(lcd->windowy <= lcd->sc_l) {
            useWind = 1;
        }
    }

    if(lcd->td_sel){
        tileData = 0x8000;
    }else{
        //Signed Data memory region
        tileData = 0x8800;
        unsig = false;
    }

    if(!useWind){
        if(lcd->tm_disp){
            backgroundMem = 0x9C00;
        }else {
            backgroundMem = 0x9800;
        }
    }else {
        if(lcd->w_tm_disp){
            backgroundMem = 0x9C00;
        }else {
            backgroundMem = 0x9800;
        }
    }

    BYTE ypos = 0;

    if(!useWind){
        ypos = lcd->scrolly + lcd->sc_l;
    }else {
        ypos = lcd->sc_l - lcd->windowy;
    }

    SHORT tileRow = (SHORT)(((BYTE)(ypos/8))*32);

    // Time to start drawing
    for(int pixel = 0; pixel < 160; pixel++) {
        BYTE xpos = pixel + lcd->scrollx;
        if (useWind) {
            if (pixel >= (lcd->windowx-7)) {
                xpos = pixel - (lcd->windowx-7);
            }
        }

        SHORT tileCol = (SHORT)(xpos / 8);
        int16_t tileNum;

        SHORT tileAddress = backgroundMem+tileRow+tileCol;
        if(unsig){
            tileNum = read_byte(mmu, tileAddress);
        }else {
            tileNum = (int16_t)read_byte(mmu, tileAddress);
        }

        SHORT tileLocation = tileData;

        if(unsig){
            tileLocation += (tileNum *16);
        }else {
            tileLocation += ((tileNum+128)*16);
        }

        BYTE line = (BYTE)(ypos % 8);
        line *= 2; // each vertical line takes up two bytes of memory
        BYTE data1 = read_byte(mmu, tileLocation + line) ;
        BYTE data2 = read_byte(mmu, (SHORT)(tileLocation + line + 1)) ;


        // pixel 0 in the tile is it 7 of data 1 and data2.
        // Pixel 1 is bit 6 etc..
        int colourBit = xpos % 8 ;
        colourBit -= 7 ;
        colourBit *= -1 ;

        // combine data 2 and data 1 to get the colour id for this pixel
        // in the tile
        int colourNum = (data2 & (1 <<colourBit))? 1: 0;
        colourNum <<= 1;
        colourNum |= (data1 & (1 <<colourBit))? 1: 0 ;

        // now we have the colour id get the actual
        // colour from palette 0xFF47
        BYTE col = GetColour(mmu, (BYTE)colourNum, 0xFF47) ;
        int red = 0;
        int green = 0;
        int blue = 0;

        // setup the RGB values
        switch(col)
        {
            case WHITE:	red = 255; green = 255 ; blue = 255; break ;
            case LIGHT_GRAY:red = 0xCC; green = 0xCC ; blue = 0xCC; break ;
            case DARK_GRAY:	red = 0x77; green = 0x77 ; blue = 0x77; break ;
            default: break;
        }

        int finaly = lcd->sc_l ;

        // safety check to make sure what im about
        // to set is int the 160x144 bounds
        if ((finaly<0)||(finaly>143)||(pixel<0)||(pixel>159))
        {
            //printf("pixel: 0x%x finaly: 0x%x\n", pixel, finaly);
            continue ;
        }

		//printf("pixel: %x finaly: %x\n", pixel, finaly);
        //printf("Red: %x Green: %x Blue: %x\n", red, blue, green);
      
		/*if(red != 0){
    		draw_screen(lcd);
		}*/

	
        lcd->screen[pixel][finaly][0] = red;
        lcd->screen[pixel][finaly][1] = green;
        lcd->screen[pixel][finaly][2] = blue;
    }
}

void RenderSprites(MMU *mmu){
    LCD *lcd = mmu->lcd;

    for(int ind = 0; ind < 40; ind++){
		BYTE index = ind * 4;
        SPRITE *newsprite;
        newsprite->spriteY = read_byte(mmu, 0xFE00+index) - 16;
        newsprite->spriteX = read_byte(mmu, 0xFE00+index+1)-8;
        newsprite->pattern = read_byte(mmu, 0xFE00+index+2);
        newsprite->attributes = read_byte(mmu, 0xFE00+index+3);

        int size = 8;

        if(lcd->size){
            size = 16;
        }

        if(lcd->sc_l >= newsprite->spriteY && (lcd->sc_l < (newsprite->spriteY + size))){
            int line = lcd->sc_l - newsprite->spriteY;

            if(newsprite->yflip){
                line -= size;
                line *= -1;
            }

            line *= 2;
            SHORT address = (SHORT)(0x8000 + (newsprite->pattern * 16)) + (SHORT)line;
            BYTE data1 = read_byte(mmu, address);
            BYTE data2 = read_byte(mmu, address+1);

            for (int tilePixel = 7; tilePixel >= 0; tilePixel--)
            {
                int colourbit = tilePixel ;
                // read the sprite in backwards for the x axis
                if (newsprite->xflip)
                {
                    colourbit -= 7 ;
                    colourbit *= -1 ;
                }

                // the rest is the same as for tiles
                int colourNum =  (data2 & (1 << colourbit))? 1:0 ;
                colourNum <<= 1;
                colourNum |= (data1 & (1 << colourbit))? 1: 0 ;

                SHORT colourAddress = (SHORT) (newsprite->pallete)?0xFF49:0xFF48;
                BYTE col= GetColour(mmu, (BYTE)colourNum, colourAddress ) ;

                // white is transparent for sprites.
                if (col == WHITE)
                    continue ;

                int red = 0;
                int green = 0;
                int blue = 0;

                switch(col)
                {
                    case WHITE: red =255;green=255;blue=255;break ;
                    case LIGHT_GRAY:red =0xCC;green=0xCC ;blue=0xCC;break ;
                    case DARK_GRAY:red=0x77;green=0x77;blue=0x77;break ;
                    default: break;
                }

                int xPix = 0 - tilePixel ;
                xPix += 7 ;

                int pixel = newsprite->spriteX+xPix ;

                // sanity check
                if ((lcd->sc_l<0)||(lcd->sc_l>143)||(pixel<0)||(pixel>159))
                {
                    continue ;
                }
        		printf("Red: %x Green: %x Blue: %x\n", red, blue, green);
                lcd->screen[pixel][lcd->sc_l][0] = red;
        		lcd->screen[pixel][lcd->sc_l][1] = green;
        		lcd->screen[pixel][lcd->sc_l][2] = blue;
            }
        }
    }
}

void DrawScanLine(void *mmu) {
    LCD *lcd = ((MMU*)mmu)->lcd;
    if (lcd->bg_d)
        RenderTiles((MMU*)mmu);
    if (lcd->s_disp_en)
        RenderSprites((MMU*)mmu);
}
