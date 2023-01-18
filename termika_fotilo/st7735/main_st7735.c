// Include the library
#include "dis_st7735.h"

// Include the font
#include "Fonts/font5x7fixed_mono.h"

int main(void){
	
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
	DIS_ST7735_drawString("Hello");
	
	// Move the cursor to the next line 5px from left
	DIS_ST7735_gotoNextLine(5);
	
	DIS_ST7735_drawString("PI = ");
	DIS_ST7735_drawFloat(3.14, 0, 2);
	
	// Draw a number in a loop
	uint8_t i = 0;
	
	while(1){
		// Set cursor to the same position after each loop
		// 5px from left and 3 text lines from top
		DIS_ST7735_setCursor(5, DIS_ST7735_getLineHeight() * 3);
		
		// fillRect() is used with the same color as the background
		// to clear the previous number and draw a new one. Use a light color as yellow
		// to debug de rectangle position. The Y position is getCursorY() minus
		// a line height with 4px down to not erase the upper line
		// - width is DIS_ST7735_getTextWidth("0") * 3 because the number has maximum 3 digits (255)
		// - height parameter is DIS_ST7735_getLineHeight()
		DIS_ST7735_fillRect(5, DIS_ST7735_getCursorY() - DIS_ST7735_getLineHeight() + 4, DIS_ST7735_getTextWidth("0") * 3, DIS_ST7735_getLineHeight(), ST77XX_BLACK);
		DIS_ST7735_drawInt(i, 0);
		i++;
		_delay_ms(200);
	}
}
