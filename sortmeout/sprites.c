#include <gba.h>
#include <dma.h>
#include <sprites.h>


//**********************//
// FUNCTION DEFINITIONS //
//**********************************************************************

//** Copy sprite array to OAM **//

void copyOAM (void) {
  DMA_Copy (3, (void *)OAMmem, (void *)sprites, 512, DMA_16NOW);
}


//** set all sprites off screen **//

void initSprites (void) {
  u8 i;

  for (i = 0; i < 128; i++) {
    turnOffSprite (&sprites[i]);

    //sprites[loop].attribute0 = 160;	
    //sprites[loop].attribute1 = 240;    
  }  
}


// ** move a sprite ** //

void moveSprite (OAMEntry* sp, s16 x, s16 y) {
  if(x < 0)			//if it is off the left correct
    x = 512 + x;
  if(y < 0)			//if off the top correct
    y = 256 + y;
  
  sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
  sp->attribute1 = sp->attribute1 | x;
  
  sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
  sp->attribute0 = sp->attribute0 | y;
}


void turnOffSprite (OAMEntry *sprite) {
  sprite -> attribute0 |= SIZE_DOUBLE;
}
