// sprite test

#include <gba.h>	//GBA register definitions
#include <keypad.h>     //button registers
#include <dispcnt.h>    //REG_DISPCNT register #defines
#include <spritebleh.h>       //my generic sprite header file
#include <block1.h>  //pacman sprite image data and palette

//create an OAM variable and make it point to the address of OAM
u16* OAM = (u16*)0x7000000;

//create the array of sprites (128 is the maximum)
OAMEntry sprites[128];

//create the rotation and scaling array (overlaps the OAMEntry array memory)
pRotData rotData = (pRotData)sprites;

//x and y sprite position variables
s16 xpos = 100;
s16 ypos = 60;

//Copy our sprite array to OAM
void CopyOAM()
{
	u16 loop;
	u16* temp;
	temp = (u16*)sprites;
	for(loop = 0; loop < 128*4; loop++)
	{
		OAM[loop] = temp[loop];
	}
}

//Set sprites to off screen
void InitializeSprites()
{
	u16 loop;
	for(loop = 0; loop < 128; loop++)
	{
		sprites[loop].attribute0 = 160;  //y to > 159
		sprites[loop].attribute1 = 240;  //x to > 239
	}
}

//wait for the screen to stop drawing
void WaitForVsync()
{
	while((volatile u16)REG_VCOUNT != 160){}
}

void GetInput()
{
	if(!(*KEYS & KEY_UP))                   //if the UP key is pressed
	{
        	if(ypos > 0)
        		ypos--;
	}
	if(!(*KEYS & KEY_DOWN))                 //if the DOWN key is pressed
	{
        	if(ypos < 144)
        		ypos++;
	}
	if(!(*KEYS & KEY_LEFT))                 //if the LEFT key is pressed
	{
        	if(xpos > 0)
        		xpos--;
	}
	if(!(*KEYS & KEY_RIGHT))                //if the RIGHT key is pressed
	{
        	if(xpos < 224)
        		xpos++;
	}
	if(!(*KEYS & KEY_A))                	//if the A key is pressed
	{}
	if(!(*KEYS & KEY_B))                	//if the B key is pressed
	{}
	if(!(*KEYS & KEY_L))                	//if the L key is pressed
	{}
	if(!(*KEYS & KEY_R))                	//if the R key is pressed
	{}
	if(!(*KEYS & KEY_SELECT))               //if the SELECT key is pressed
	{}
	if(!(*KEYS & KEY_START))                //if the START key is pressed
	{}
}

//move the sprite
void MoveSprite(OAMEntry* sp, int x, int y)
{
	if(x < 0)			//if it is off the left correct
		x = 512 + x;
	if(y < 0)			//if off the top correct
		y = 256 + y;

	sp->attribute1 = sp->attribute1 & 0xFE00;  //clear the old x value
	sp->attribute1 = sp->attribute1 | x;

	sp->attribute0 = sp->attribute0 & 0xFF00;  //clear the old y value
	sp->attribute0 = sp->attribute0 | y;
}

//int AgbMain()
int main()
{
	u16 loop;       //generic loop variable

       	SetMode(MODE_1 | OBJ_ENABLE | OBJ_MAP_1D); //set mode 1 and enable sprites and 1d mapping

	for(loop = 0; loop < 256; loop++)          //load the palette into memory
		OBJPaletteMem[loop] = block1Palette[loop];

	InitializeSprites();                       //set all sprites off screen (stops artifact)

       	sprites[0].attribute0 = COLOR_256 | SQUARE | ypos; //setup sprite info, 256 colour, shape and y-coord
        sprites[0].attribute1 = SIZE_16 | xpos;            //size 16x16 and x-coord
        sprites[0].attribute2 = 0;                         //pointer to tile where sprite starts

        for(loop = 0; loop < 128; loop++)               //load sprite image data
       	{
       		OAMData[loop] = block1Data[loop];
       	}

	while(1)                                //main loop
        {
		GetInput();                     //the input function, checks for key presses
		MoveSprite(&sprites[0], xpos, ypos);
		WaitForVsync();			//waits for the screen to stop drawing
		CopyOAM();			//Copies sprite array into OAM.
	}
}

