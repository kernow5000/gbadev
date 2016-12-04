#include <gba.h>
#include <dispcnt.h>
#include <dma.h>
#include <lz77.h>


u16 *theVideoBuffer = BackBuffer;


void flip (void) {
  if(REG_DISPCNT & backbuffer) //back buffer is the current buffer so we need to switch it to the front buffer
    { 
      REG_DISPCNT &= ~backbuffer;  //flip active buffer to front buffer by clearing back buffer bit
      theVideoBuffer = BackBuffer;    //now we point our drawing buffer to the back buffer
    }
  else                               //front buffer is active so switch it to backbuffer
    { 
      REG_DISPCNT |= backbuffer; //flip active buffer to back buffer by setting back buffer bit
      theVideoBuffer = VideoBuffer; //now we point our drawing buffer to the front buffer
    }
  
  return;
}


void putPixel (u8 x, u8 y, u8 palIndex) {
  u8 t, tx;

  t = (x & 0x01);
  tx = x >> 1;      //Same as divide by 2 but faster(if compiler don't optimize it that way itself)
  if (t == 0) theVideoBuffer [(y * 120) + tx] = (theVideoBuffer [(y * 120) + tx] & 0xFF00) | palIndex;
  if (t == 1) theVideoBuffer [(y * 120) + tx] = (theVideoBuffer [(y * 120) + tx] & 0x00FF) | (palIndex << 8);
}


u8 getPixel (u16 *buf, u8 x, u8 y) {

}


//drawBG
//Load palette and decompress image data into VideoBuffer

void drawBG (u16 *pal, u16 *data) {
  DMA_Copy (3, (void *)BGPaletteMem, (void *)pal, 128, DMA_32NOW);
  LZ77UnCompVRAM ((void *)data, (void *)theVideoBuffer);
}


void line (u8 x, u8 y, u8 x2, u8 y2, u8 paletteIndex) {
  int i;
  int steep = 1;
  int sx, sy;  /* step positive or negative (1 or -1) */
  int dx, dy;  /* delta (difference in X and Y between points) */
  int e;

  /* * inline swap. On some architectures, the XOR trick may be faster */
  
  int tmpswap;
  #define SWAP(a,b) tmpswap = a; a = b; b = tmpswap;

  /* * optimize for vertical and horizontal lines here */
  dx = abs(x2 - x);
  sx = ((x2 - x) > 0) ? 1 : -1;
  dy = abs(y2 - y);
  sy = ((y2 - y) > 0) ? 1 : -1;
  if (dy > dx) {
    steep = 0;
    SWAP (x, y);
    SWAP (dx, dy);
    SWAP (sx, sy);
  }

  e = (dy << 1) - dx;
  for (i = 0; i < dx; i++) {
    if (steep) { 
      putPixel (x, y, paletteIndex);
    } 
    else { 
      putPixel (y, x, paletteIndex); 
    } 
    while (e >= 0) { 
      y += sy;
      e -= (dx << 1);
    }
    x += sx;
    e += (dy << 1);
  }
}

