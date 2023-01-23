// Include the library
#include "dis_st7735.h"

// Include the font
#include "fonts/classicGFXfont.h"
#define ZOOM 7

void setup()
{
	DIS_ST7735_displayInit();
  DIS_ST7735_setRotation(ROTATE_DEGREE_270);
	DIS_ST7735_fillScreen(ST77XX_BLACK);
	DIS_ST7735_fillRect(0, 0, 159, 20, 0x1326);
	DIS_ST7735_drawRect(0, 0, 159, 20, ST77XX_YELLOW);

	DIS_ST7735_setFont(&classicGFXfont);
	DIS_ST7735_setFontColor(ST77XX_RED);
  DIS_ST7735_setFontSize(2, 2);
	DIS_ST7735_setCursor(100, 3);
  DIS_ST7735_drawString(F("Make:"));

	DIS_ST7735_setFontColor(ST77XX_WHITE);
  DIS_ST7735_setFontSize(1, 1);
	DIS_ST7735_setCursor(3, 7);
  DIS_ST7735_drawString(F("TERMIKA FOTILO"));

	DIS_ST7735_setFontColor(0xfa8a);
	DIS_ST7735_setCursor(0, 112);
  DIS_ST7735_drawString(F("Init..."));
	DIS_ST7735_setCursor(0, 120);
  DIS_ST7735_drawString(F("Please wait"));

	DIS_ST7735_fillRect(0, 27, 16+2, 4+2, 0x000c);
	DIS_ST7735_drawRect(0, 27, 16+2, 4+2, 0x7bef);
	DIS_ST7735_fillRect(0, 40, 16*ZOOM+2, 4*ZOOM+2, 0x000c);
	DIS_ST7735_drawRect(0, 40, 16*ZOOM+2, 4*ZOOM+2, 0x7bef);
	DIS_ST7735_fillRect(0, 75, 15*ZOOM+2, 3*ZOOM+2, 0x000c);
	DIS_ST7735_drawRect(0, 75, 15*ZOOM+2, 3*ZOOM+2, 0x7bef);

/*
	// Init function
	DIS_ST7735_displayInit();
	
	// Clear the display with the desired background color
	DIS_ST7735_fillScreen(ST77XX_BLACK);
	
	// Set the font
	DIS_ST7735_setFont(&Font5x7FixedMono);
	
	// Set the font color
	DIS_ST7735_setFontColor(ST77XX_RED);
	
	// Set the cursor 5px from the left and a distance 
	// equal with the text height from the top
	DIS_ST7735_setCursor(5, DIS_ST7735_getLineHeight());
	
	// Draw this original string
	DIS_ST7735_drawString(F("Ich komme aus dem\n Flashspeicher!!!"));
	
	// Move the cursor to the next line 5px from left
	DIS_ST7735_gotoNextLine(5);
	
	DIS_ST7735_drawString("PI = ");
	DIS_ST7735_drawFloat(3.14, 0, 2);
*/
}

void loop()
{	
	// Draw a number in a loop
	uint8_t i = 0;
	
	while(1){
		// Set cursor to the same position after each loop
		// 5px from left and 3 text lines from top
		DIS_ST7735_setCursor(5, DIS_ST7735_getLineHeight() * 4);
		
		// fillRect() is used with the same color as the background
		// to clear the previous number and draw a new one. Use a light color as yellow
		// to debug de rectangle position. The Y position is getCursorY() minus
		// a line height with 4px down to not erase the upper line
		// - width is DIS_ST7735_getTextWidth("0") * 3 because the number has maximum 3 digits (255)
		// - height parameter is DIS_ST7735_getLineHeight()
		DIS_ST7735_fillRect(5, DIS_ST7735_getCursorY(), DIS_ST7735_getTextWidth("0") * 3, DIS_ST7735_getLineHeight(), ST77XX_BLACK);
		DIS_ST7735_drawInt(i, 0);
		i++;
		_delay_ms(200);
	}
}
