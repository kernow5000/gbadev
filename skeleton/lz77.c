#include <lz77.h>


//**********************//
// FUNCTION DEFINITIONS //
//**********************************************************************


// ** UnCompress LZ77 Data to WRAM ** //

inline void LZ77UnCompWRAM(void *source, void *dest) {
  asm("mov r0, %0\n"
      "mov r1, %1\n"
      "swi 0x110000\n"
      :
      :"r" (source), "r" (dest)
      :"r0", "r1" );
}


// ** UnCompress LZ77 Data to VRAM ** //

inline void LZ77UnCompVRAM(void *source, void *dest) {
  asm("mov r0, %0\n"
      "mov r1, %1\n"
      "swi 0x120000\n"
      :
      :"r" (source), "r" (dest)
      :"r0", "r1" );
}
