#include <gba.h>	//GBA register definitions
#include <keypad.h>     //button registers
#include <dispcnt.h>    //REG_DISPCNT register #defines
#include <lz77.h>       // built-in LZ77 graphics decompression


// the external picture data binaries
extern const u16 titlescreen[];
extern const u16 titlescreenPalette[];
  

int main()
{
  
  SetMode(MODE_4 | BG2_ENABLE); //set mode 3

  
  // setup palette, copy the titlescreens palette table into the GBA's palette memory
  memcpy (BGPaletteMem, titlescreenPalette, 512);
  
  // decompress the background into VRAM
  LZ77UnCompVRAM ((void *)titlescreen, (void *)VideoBuffer);
  
  // spin forever, game loop
  while(1) 
    {
    }
  
  
  return 0;
}
