/*  simple Hello World, for cc65, for NES
 *  writing to the screen with rendering disabled
 *  using neslib
 *  Doug Fraker 2018
 */

#include "LIB/neslib.h"
#include "LIB/nesdoug.h"

#define BLACK 0x0f
#define DK_GY 0x27
#define LT_GY 0x38
#define WHITE 0x30
// there's some oddities in the palette code, black must be 0x0f, white must be 0x30

#pragma bss-name(push, "ZEROPAGE")

// GLOBAL VARIABLES
// all variables should be global for speed
// zeropage global is even faster

unsigned char i;
unsigned char ix;
unsigned char iy;
unsigned char pos;
unsigned char src;
unsigned char dest;
unsigned char clr;

const unsigned char text[]="Hail Satan!"; // zero terminated c string

const unsigned char WIDTH = 16;
const unsigned char HEIGHT = 15;

unsigned char scr[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6
};

const unsigned char palette[]={
BLACK, DK_GY, LT_GY, WHITE,
0,0,0,0,
0,0,0,0,
0,0,0,0
};

void draw_bg(void);
void update_bg(void);

void main (void) {
  // vram_adr and vram_put only work with screen off
  pal_bg(palette); //  load the BG palette
  set_vram_buffer(); // points ppu update to vram_buffer, do this at least once

  ppu_on_all(); //  turn on screen

  clear_vram_buffer(); // resets the index to zero, to start filling the buffer

  while(1) {
    ppu_wait_nmi(); // wait

    update_bg();
    draw_bg();
  }
}

void update_bg(void) {
  //sizeof(scr) - WIDTH
  for(i = 223; i > 0; --i) {
    // src is next row
    src = scr[i + WIDTH];

    if(rand8() > 64) {
      dest = i - 1;
    } else {
      dest = i;
    }
    
    if(rand8() > 64) {
      clr = src - 1;
    } else {
      clr = src;
    }

    scr[dest] = clr;
  }
}

void draw_bg(void) {
  ppu_off(); // screen off

  clear_vram_buffer(); // resets the index to zero, to start filling the buffer

  vram_adr(NAMETABLE_A); // start in the top left corner

  // draw every block twice
  for(iy=0; iy < HEIGHT; ++iy) {
    // draw each line twice
    for(i = 0; i < 2; ++i) {
      for(ix=0; ix < WIDTH; ++ix) {
        pos = (iy << 4) + ix;
        // draw each column twice
        vram_put(scr[pos]);
        vram_put(scr[pos]);
      }
    }
  }

  vram_adr(NTADR_A(10,14)); // screen is 32 x 30 tiles
  vram_write(text,sizeof(text));

  ppu_on_all(); //  turn on screen
}
