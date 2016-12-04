#include <gba.h>
#include <collision.h>
#include <boolean.h>

//a cBox is 4 u8 ints representing the four corners of a collision box

enum BOOL checkSpriteCollision (u8 *cBox, u8 x, u8 y, u8 *cBox2, u8 x2, u8 y2) {
  if (x + cBox[LEFT] > x2 + cBox2[RIGHT])
    return FALSE;
  else
    if (x2 + cBox2[LEFT] > x + cBox[RIGHT])
      return FALSE;
    else
      if (y + cBox[TOP] > y2 + cBox2[BOTTOM])
	return FALSE;
      else
	if (y2 + cBox2[TOP] > y + cBox[BOTTOM])
	  return FALSE; 
    
  return TRUE;   
}
