#include <gba.h>
#include <interrupts.h>

u32 FRAMECOUNT = 0;
u8 DRAWSPRITES = 0;


//**********************//
// FUNCTION DEFINITIONS //
//**********************************************************************

//enable interrupts

void enableInterrupts (u16 interrupts) {
  REG_IME = 0;  //probably not necessary but not a good idea to change interupt registers when an interupt is ocuring
    
  if(interrupts | INT_VBLANK)
    REG_DISPSTAT |= 0x8;
  
  if(interrupts | INT_HBLANK)
    REG_DISPSTAT |= 0x10;
    
  REG_IE |= interrupts;
  
  REG_IME = 1;  
}


//disable interrupts

void DisableInterrupts (u16 interrupts) {
  REG_IE &= ~interrupts;
  
  if(interrupts == INT_ALL) //this is were that ALL comes in
    REG_IME = 0;  //disable all interupts;
}



// ** INTERRUPT FUNCTIONS **

void VBLANK (void) {
  FRAMECOUNT++;
  if (DRAWSPRITES == 1) {
    copyOAM ();
    DRAWSPRITES = 0;
  }
  REG_IF |= INT_VBLANK;
}


void HBLANK(void) {
	
  REG_IF |= INT_HBLANK;
}


void DMA0 (void) {
  REG_IF |= INT_DMA0;
}


void DMA1 (void) {
  REG_IF |= INT_DMA1;
}


void DMA2 (void) {
  REG_IF |= INT_DMA2;
}


void DMA3 (void) {
  REG_IF |= INT_DMA3;
}


void VCOUNT (void) {
  REG_IF |= INT_VCOUNT;
}


void TIMER0 (void) {	
  REG_IF |= INT_TIMER0;
}


void TIMER1 (void) {
  REG_IF |= INT_TIMER1;
}


void TIMER2 (void) {
  REG_IF |= INT_TIMER2;
}


void TIMER3 (void) {
  REG_IF |= INT_TIMER3;
}


void KEYBOARD (void) {
  REG_IF |= INT_TIMER3;
}


void CART (void) {
  REG_IF |= INT_CART;
}


void COMMUNICATION (void) {
  REG_IF |= INT_COMMUNICATION;
}
