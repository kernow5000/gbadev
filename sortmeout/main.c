//by GreenGianT 2004

//*********************//
// includes            //
//**********************************************************************

#include <gba.h>		//GBA register definitions
#include <dispcnt.h>		//REG_DISPCNT register #defines
#include <dma.h>
#include <keypad.h>      	//gba button registers
#include <sprites.h>		//sprite definitions
#include <bg.h>                 //backgrounds
#include <2d.h>
#include <rand.h>

#include <interruptTable.h>
#include <interrupts.h>

#include <boolean.h>            //a BOOL datatype (TRUE/FALSE)

#include <quicksort.h>

#include <collision.h>
#include <gfxData.h>

#define WAIT_FOR_VSYNC while((volatile u16)REG_VCOUNT != 160){}


//*********************//
// CONSTANTS           //
//**********************************************************************

#define X 0
#define Y 1


//SCORE STUFF

#define SCORE_LABEL_SPRITE  0   //sprite numbers
#define SCORE_DIGIT_SPRITE1 1
#define SCORE_DIGIT_SPRITE2 2

#define SCORE_X 207             //location of score on screen
#define SCORE_Y 0


#define FIRST_SHOT_SPRITE   3
#define MAX_SHOTS 9

#define FIRST_ALIEN_SPRITE  3 + MAX_SHOTS
#define MAX_ALIENS 8
#define ALIEN_WAIT_TIME 150

#define SCORE_LABEL_TILENUM  512
#define ZERO_TILENUM         528
#define SHOT_FRAME1_TILENUM  608
#define ALIEN1_FRAME1        736       //TILENUMBERS for the alien sprites


//*********************//
// STRUCTS             //
//**********************************************************************

struct game_s {
  u8 level;
  u8 score;
} game;

struct path_s {
  u8 length;            //number of pairs of coords in this path
  s16 coords[128][2];   //path coords
  u8 currentPosn;
};

struct alien_s {
  enum BOOL alive;
  s16 x, y;             //screen coords
  u8 crater;            //what crater the alien is on/ moving to
  enum BOOL moving;     //is the alien moving, if not then it is waiting
  u16 waiting;          //how many frames the alien has been "waiting" above a crater
  struct path_s path;   //the path that this alien will follow
  u8 spriteNum;         //indexes the sprites array
  s8 frames;            //-1 for not animated
  u8 frameNum;          //current frame number
} aliens [MAX_ALIENS];

struct shot_s {
  enum BOOL alive;
  s16 x, y;
  struct path_s path;   //the path that this shot will follow
  u8 spriteNum;
  s8 frames;
  u8 frameNum;
} shots [MAX_SHOTS];


//*********************//
// function prototypes //
//**********************************************************************

void getInput (void);
void doTitlescreen (void);
void loadSprites (void);

void newGame (void);

void loadLevel (u8 level);
void drawHUD (void);
void updateScore (u8 score);

void doCollision (void);

struct path_s newPath (s16 startX, s16 startY, s16 endX, s16 endY);

void initShots (void);
void newShot (s16 targetX, s16 targetY);
void moveShots (void);
void removeShot (struct shot_s *shot);

void initAliens (void);
void newAlien (void);
void sortAliens (void);
void moveAliens (void);
void killAlien (u8 alien);

void animate (void);


//*********************//
// GLOBALS VARS        //
//**********************************************************************

enum BOOL upLock = FALSE, downLock = FALSE, leftLock = FALSE, rightLock = FALSE, aLock = FALSE, bLock = FALSE, lLock = FALSE, rLock = FALSE, startLock = FALSE, selectLock = FALSE;

//crater co-ords so the aliens know where to move to and when to fire your shot.  different for each level
s16 CRATERS[9][2] = { {62,85}, {126,85}, {192,87}, {28,100}, {134,108}, {218,104}, {43,128}, {107,142}, {176,128} };

//collision boxes
//for aliens use CBOX[levelNum-1] for the aliens
//for shots use CBOX[shot[currentShot].frame]

u8 SHOT_CBOX[4][4] = {
{10,22,10,22},
{8,24,8,24},
{8,24,8,24},
{8,24,8,24}
};

u8 ALIEN_CBOX[6][4] = {
  {15,49,1,29},
  {0,64,0,32},
  {0,64,0,32},
  {0,64,0,32},
  {0,64,0,32},
  {0,64,0,32}
};

u8 CURRENTSHOTS = 0;

extern u16 *theVideoBuffer;
extern u32 FRAMECOUNT;
extern u8 DRAWSPRITES;

u16 ALIEN_FIRSTTILE = ALIEN1_FRAME1;   //update ALIEN_FIRST_TILE on a level change


//*********************//
// MAIN PROGRAM        //
//**********************************************************************

int main (void) {
  u8 score = 0;
  u16 bing = 0;

  enableInterrupts (INT_VBLANK);

  SetMode (MODE_4 | OBJ_ENABLE | OBJ_MAP_1D | BG2_ENABLE);
 
  doTitlescreen ();
  loadSprites ();


  newGame ();
  loadLevel (1);

  initSprites ();    //put these two lines into "loadLevel ()"

  drawHUD ();
  initAliens ();
  initShots ();

  newAlien ();
  newAlien ();
  newAlien ();

  while (1) {
    //WAIT_FOR_VSYNC;
    for (bing = 0; bing < 2500; bing++) ;   //just a hack for some delay

    moveShots ();
    moveAliens ();
    animate ();

    //if (CURRENTSHOTS > 0)
    //  doCollision ();

    updateScore (score++);
    if (score == 100)
      score = 0;

    getInput ();
  }

  exit (0);
}



//**********************//
// FUNCTION DEFINITIONS //
//**********************************************************************

// ** handle user input **

void getInput (void) {  
  if (!(*KEYS & KEY_UP) && upLock == FALSE) {        //** DPAD
    upLock = TRUE;
    newShot (CRATERS[1][X], CRATERS[1][Y]);
  }

  if (*KEYS & KEY_UP) upLock = FALSE;
  

  if (!(*KEYS & KEY_DOWN) && downLock == FALSE) {
    downLock = TRUE;
    newShot (CRATERS[7][X], CRATERS[7][Y]);
  }
  
  if (*KEYS & KEY_DOWN) downLock = FALSE;


  if (!(*KEYS & KEY_LEFT) && leftLock == FALSE) {
    leftLock = TRUE;
    newShot (CRATERS[3][X], CRATERS[3][Y]);
  }

  if (*KEYS & KEY_LEFT) leftLock = FALSE;

  
  if (!(*KEYS & KEY_RIGHT) && rightLock == FALSE) {
    rightLock = TRUE;
    newShot (CRATERS[5][X], CRATERS[5][Y]);
  }

  if (*KEYS & KEY_RIGHT) rightLock = FALSE;
  

  if(!(*KEYS & KEY_A) && aLock == FALSE) {          //** A and B
    aLock = TRUE;
    newShot (CRATERS[2][X], CRATERS[2][Y]);
  }

  if (*KEYS & KEY_A) aLock = FALSE;


  if(!(*KEYS & KEY_B) && bLock == FALSE) {
    bLock = TRUE;
    newShot (CRATERS[0][X], CRATERS[0][Y]);
  }

  if (*KEYS & KEY_B) bLock = FALSE;

  
  if(!(*KEYS & KEY_L) && lLock == FALSE) {          //** SHOULDER BUTTONS
    lLock = TRUE;
    newShot (CRATERS[6][X], CRATERS[6][Y]);
  }

  if (*KEYS & KEY_L) lLock = FALSE;
  

  if(!(*KEYS & KEY_R) && rLock == FALSE) {
    rLock = TRUE;
    newShot (CRATERS[8][X], CRATERS[8][Y]);
  }

  if (*KEYS & KEY_R) rLock = FALSE;


  if(!(*KEYS & KEY_START) && startLock == FALSE) {     //** START
    startLock = TRUE;
    newShot (CRATERS[4][X], CRATERS[4][Y]);
  }

  if (*KEYS & KEY_START) startLock = FALSE;


  if(!(*KEYS & KEY_SELECT) && selectLock == FALSE) {    //** SELECT
    selectLock = TRUE;
    //newAlien ();
  }

  if (*KEYS & KEY_SELECT) selectLock = FALSE;
}


//doTitlescreen ()
//fade in the titlescreen, wait for a keypress, then fade out

void doTitlescreen (void) {
  drawBG ((u16*)titlescreenPalette, (u16*)titlescreen);

  flip ();

  while (*KEYS & KEY_START) { WAIT_FOR_VSYNC; }
  while (!(*KEYS & KEY_START)) { WAIT_FOR_VSYNC; }
  
}


//loadSprites ()
//load the sprite palette and put the sprite data into OAMData

void loadSprites (void) {
  int posn = 16384;
 
  DMA_Copy (3, (void *)OBJPaletteMem, (void *)spritePalette, 128, DMA_32NOW);   //load palette

  LZ77UnCompVRAM ((void *)scoreData, (void *)OAMData + posn);                    //load sprites
  posn += 32 * 16;
  LZ77UnCompVRAM ((void *)numsData, (void *)OAMData + posn);
  posn += 16 * 160;
  LZ77UnCompVRAM ((void *)shotData, (void *)OAMData + posn);
  posn += 32 * 128;
  LZ77UnCompVRAM ((void *)alien1_1, (void *)OAMData + posn);                    
}


//newGame ()
//init the game struct with level and score info

void newGame (void) {
  game.level = 1;
  game.score = 0;
}


//loadLevel ()
//loads a new bg image for the given level

void loadLevel (u8 level) {
  switch (level) {
  case 1: {
    drawBG ((u16*)level1Palette, (u16*)level1);
    flip ();

    ALIEN_FIRSTTILE = ALIEN1_FRAME1;
    break;
  }
  }
}


//drawHUD ()
//draw the score label and score of 00

void drawHUD (void) {
  sprites[SCORE_LABEL_SPRITE].attribute0 = COLOR_256 | TALL | SCORE_Y;
  sprites[SCORE_LABEL_SPRITE].attribute1 = SIZE_32 | SCORE_X - 32;
  sprites[SCORE_LABEL_SPRITE].attribute2 = SCORE_LABEL_TILENUM;

  sprites[SCORE_DIGIT_SPRITE1].attribute0 = COLOR_256 | SQUARE | SCORE_Y;
  sprites[SCORE_DIGIT_SPRITE1].attribute1 = SIZE_16 | SCORE_X;

  sprites[SCORE_DIGIT_SPRITE2].attribute0 = COLOR_256 | SQUARE | SCORE_Y;
  sprites[SCORE_DIGIT_SPRITE2].attribute1 = SIZE_16 | SCORE_X + 16;

  updateScore (0);
}


//updateScore ()
//draw/redraw the score

void updateScore (u8 score) {
  u8 tens = 0, units = 0;
  u8 i = score;
 
  while (i >= 10) {
    tens++;
    i -= 10;
  }
  
  if ((i > 0) && (i < 10))
    units = i; 
    
  sprites[SCORE_DIGIT_SPRITE1].attribute2 = ZERO_TILENUM + (8 * tens);
  sprites[SCORE_DIGIT_SPRITE2].attribute2 = ZERO_TILENUM + (8 * units);

  DRAWSPRITES = TRUE;
}


//doCollision ()
//check through all shots and all aliens to see if there has been a collision, if so deal with it.

void doCollision (void) {
  u8 alien, shot;

  for (alien = 0; alien < MAX_ALIENS; alien++) {
    if (aliens[alien].alive == TRUE) {
      for (shot = 0; shot < MAX_SHOTS; shot++) {
	if (shots[shot].alive == TRUE) {
	  if (checkSpriteCollision (ALIEN_CBOX[game.level-1], aliens[alien].x, aliens[alien].y, SHOT_CBOX[shots[shot].frameNum], shots[shot].x, shots[shot].y) == TRUE) {
	    removeShot (&shots[shot]);          //COLLISION DETECTED!
	    killAlien (alien);
	  }
	}
      }
    }
  }
}


//newPath ()
//create a new path for an alien or shot to follow

struct path_s newPath (s16 x, s16 y, s16 x2, s16 y2) {
  struct path_s path;
  int i;
  int steep = 1;
  int sx, sy;    /* step positive or negative (1 or -1) */
  int dx, dy;    /* delta (difference in X and Y between points) */
  int e;
  int bing = 0;
  int STEP = 2;

  path.length = 0;

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
      if (bing % STEP == 0) {
	path.coords[path.length][X] = x;
	path.coords[path.length][Y] = y;
	path.length ++;
      }
    } 
    else { 
      if (bing % STEP == 0) {
	path.coords[path.length][X] = y;
	path.coords[path.length][Y] = x;
	path.length ++;
      }
    }

    if (++bing == STEP)
      bing = 0;

    while (e >= 0) { 
      y += sy;
      e -= (dx << 1);
    }

    x += sx;
    e += (dy << 1);
  }

  path.currentPosn = 0;

  return path;
}


//initShots ()
//initialises the shots array

void initShots (void) {
  u8 i;
  
  for (i = 0; i < MAX_SHOTS; i++) {
    shots[i].alive = FALSE;
  }
}


//newShot ()
//fire a new shot at the given target X and Y

void newShot (s16 targetX, s16 targetY) {
  u8 i;
  
  for (i = 0; i < MAX_SHOTS; i++) {
    if (shots[i].alive == FALSE) {
      shots[i].alive = TRUE;
      shots[i].x = 104;
      shots[i].y = 128;

      shots[i].path = newPath (shots[i].x, shots[i].y, targetX - 16, targetY - 32);

      shots[i].frames = 4;
      shots[i].frameNum = 0;
      shots[i].spriteNum = FIRST_SHOT_SPRITE + i;

      sprites[shots[i].spriteNum].attribute0 = COLOR_256 | SQUARE | shots[i].y;
      sprites[shots[i].spriteNum].attribute1 = SIZE_32 | shots[i].x;
      sprites[shots[i].spriteNum].attribute2 = SHOT_FRAME1_TILENUM;

      DRAWSPRITES = TRUE;

      CURRENTSHOTS++;

      return;
    }
  }
}


//moveShots ()
//moves all the shots to their next position

void moveShots (void) {
  u8 i;

  for (i = 0; i < MAX_SHOTS; i++) {
    if (shots[i].alive == TRUE) {
      shots[i].path.currentPosn++;
      shots[i].x = shots[i].path.coords[shots[i].path.currentPosn][0];
      shots[i].y = shots[i].path.coords[shots[i].path.currentPosn][1];
     
      if (shots[i].path.currentPosn == shots[i].path.length -1) {
	removeShot (&shots[i]);

	/* shots[i].alive = FALSE; */
/* 	turnOffSprite (&sprites[shots[i].spriteNum]); */
/* 	CURRENTSHOTS--; */
      }
      else
	moveSprite (&sprites[shots[i].spriteNum], shots[i].x, shots[i].y);

      DRAWSPRITES = TRUE;
    }
  }
}


//removeShot ()
//remove a shot from the screen because it hit something or it reached the end of its path

void removeShot (struct shot_s *shot) {
  shot->alive = FALSE;
  turnOffSprite (&sprites[shot->spriteNum]);
  CURRENTSHOTS--;
}


//initAliens ()
//make sure the aliens array is initialised

void initAliens (void) {
  u8 i;

  for (i = 0; i < MAX_ALIENS; i++) {
    aliens[i].alive = FALSE;
    aliens[i].y = 0;
  }
}


//newAlien ()
//create a new alien and place it on the screen

void newAlien (void) {
  u8 i, j, k = 0, crater;
  enum BOOL NEWCRATER;

  for (i = 0; i < MAX_ALIENS; i++) {
    if (aliens[i].alive == FALSE) {
      aliens[i].alive = TRUE;
     
      do {
	crater = rand (99) * 9;
	if (crater == 9) crater--;
	NEWCRATER = TRUE;

	for (j = 0; j < MAX_ALIENS && NEWCRATER == TRUE; j++) {
	  if ((j != i) && (aliens[j].alive == TRUE))
	    if (aliens[j].crater == crater)
	      NEWCRATER = FALSE;
	}
      } while (NEWCRATER == FALSE);

   
      aliens[i].x = CRATERS[crater][0] - 32;
      aliens[i].y = CRATERS[crater][1] - 32;              //screen coords

      aliens[i].crater = crater;                          //set the aliens crater
      
      aliens[i].moving = FALSE;
      aliens[i].waiting = rand (99) * ALIEN_WAIT_TIME -1;

      aliens[i].spriteNum = i + FIRST_ALIEN_SPRITE;
      aliens[i].frames = -1;
      aliens[i].frameNum = 0;
      
      sprites[aliens[i].spriteNum].attribute0 = COLOR_256 | TALL | aliens[i].y;
      sprites[aliens[i].spriteNum].attribute1 = SIZE_64 | aliens[i].x;
      sprites[aliens[i].spriteNum].attribute2 = ALIEN_FIRSTTILE;

      DRAWSPRITES = TRUE;
      
      return;
    }    
  }
}


//sortAliens ()
//sort the aliens so that the sprite priority is correct, sort by the y value.  the lower the y value the further back the sprite

void sortAliens (void) {
  int data[MAX_ALIENS];
  int i, currentAlien;
  int newAlien = 0;

  for (i = 0; i < MAX_ALIENS; i++) {
    data[i] = aliens[i].y;
  }

  quickSort (data, 0, MAX_ALIENS-1);

  for (i = MAX_ALIENS-1; i >= 0; i--) {
    currentAlien = 0;
    while (aliens[currentAlien].y != data[i]) currentAlien++;

    aliens[currentAlien].spriteNum = FIRST_ALIEN_SPRITE + newAlien;
    newAlien++;
    if (aliens[currentAlien].alive == TRUE)
      moveSprite (&sprites[aliens[currentAlien].spriteNum], aliens[currentAlien].x, aliens[currentAlien].y);
  }
}


//moveAliens ()
//move aliens along their current path
//if an alien get to the front of the screen its gameover

void moveAliens (void) {
  u8 i, j, newCrater;
  enum BOOL NEWCRATER;

  for (i = 0; i < MAX_ALIENS; i++) {
    if ((aliens[i].alive == TRUE) && (aliens[i].moving != FALSE)) {
      aliens[i].path.currentPosn++;
      aliens[i].x = aliens[i].path.coords[aliens[i].path.currentPosn][0];
      aliens[i].y = aliens[i].path.coords[aliens[i].path.currentPosn][1];
      moveSprite (&sprites[aliens[i].spriteNum], aliens[i].x, aliens[i].y);
      DRAWSPRITES = TRUE;
      if (aliens[i].path.currentPosn == aliens[i].path.length -1)
	aliens[i].moving = FALSE;
    }
    else
      if (aliens[i].moving == FALSE) {
	aliens[i].waiting++;
	if (aliens[i].waiting > ALIEN_WAIT_TIME) {
	  aliens[i].waiting = 0;
	  aliens[i].moving = TRUE;
	  
	  do {                                              //choose a crater thats not already taken
	    newCrater = rand (FRAMECOUNT % 99) * 9;
	    if (newCrater == 9) newCrater--;
	    NEWCRATER = TRUE;

	    for (j = 0; j < MAX_ALIENS && NEWCRATER == TRUE; j++) {
	      if ((aliens[j].alive == TRUE) && (aliens[j].crater == newCrater))
		NEWCRATER = FALSE;
	    }
	  } while (NEWCRATER == FALSE);

	  aliens[i].crater = newCrater;

	  aliens[i].path = newPath (aliens[i].x, aliens[i].y, CRATERS[aliens[i].crater][0] - 32, CRATERS[aliens[i].crater][1] - 32);
	}	  
      }
  }

  sortAliens ();
  DRAWSPRITES = TRUE;
}


//killAlien ()
//remove an alien that has been hit and award the score

void killAlien (u8 alien) {
  aliens[alien].alive = FALSE;
  aliens[alien].y = 0;
  turnOffSprite (&sprites[aliens[alien].spriteNum]);
 
  // ** AWARD SCORE **

  DRAWSPRITES = TRUE;
}


//animate ()
//check frame counter, update animated sprites on every nth frame

void animate (void) {
  //if FRAMECOUNT % 5 then ANIMATE == TRUE
  //if ANIMATE == TRUE then increment frame number of everything thats animated
  //set ANIMATE back to FALSE
}
