#include <gba.h>	//GBA register definitions
#include <keypad.h>     //button registers
#include <dispcnt.h>    //REG_DISPCNT register #defines
#include <spritebleh.h> //my generic sprite header file ? .. needed?


// check midnight commander or bin2o for these names
extern const u16 bloke_pal[];
extern const u16 bloke_data[];
  


int main()
{
  
  SetMode(MODE_4 | BG2_ENABLE); //set mode 3
    
  // setup palette
  memcpy (BGPaletteMem, bloke_pal, 512);

  // copy picture to VRAM
  memcpy (VideoBuffer, bloke_data, 38400);
  
  
  // spin forever, game loop
  while(1) 
    {
    }
  
  
  return 0;
}
