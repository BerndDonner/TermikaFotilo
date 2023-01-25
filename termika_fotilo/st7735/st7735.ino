// Include the library
#include "dis_st7735.h"

// Include the font
#include "fonts/classicGFXfont.h"
#define ZOOM 7

const int8_t MINTEMP = -5;
const uint8_t MAXTEMP = 30;

const uint16_t PROGMEM colormap[0x100] = {
  0x7800, 0x7800, 0x7820, 0x7840, 0x7860, 0x7880, 0x78a0, 0x78c0, 0x78e0,
  0x7900, 0x7920, 0x7940, 0x7960, 0x7980, 0x79a0, 0x79c0, 0x79e0, 0x7a00,
  0x7a20, 0x7a40, 0x7a60, 0x7a80, 0x7aa0, 0x7ac0, 0x7ae0, 0x7b00, 0x7b20,
  0x7b40, 0x7b60, 0x7b80, 0x7ba0, 0x7bc0, 0x7be0, 0x7be0, 0x73e0, 0x73e0,
  0x6be0, 0x6be0, 0x63e0, 0x63e0, 0x5be0, 0x5be0, 0x53e0, 0x53e0, 0x4be0,
  0x4be0, 0x43e0, 0x43e0, 0x3be0, 0x3be0, 0x33e0, 0x33e0, 0x2be0, 0x2be0,
  0x23e0, 0x23e0, 0x1be0, 0x1be0, 0x13e0, 0x13e0, 0x0be0, 0x0be0, 0x03e0,
  0x03e0, 0x03e0, 0x03e0, 0x03e0, 0x03e1, 0x03e1, 0x03e2, 0x03e2, 0x03e3,
  0x03e3, 0x03e4, 0x03e4, 0x03e5, 0x03e5, 0x03e6, 0x03e6, 0x03e7, 0x03e7,
  0x03e8, 0x03e8, 0x03e9, 0x03e9, 0x03ea, 0x03ea, 0x03eb, 0x03eb, 0x03ec,
  0x03ec, 0x03ed, 0x03ed, 0x03ee, 0x03ee, 0x03ef, 0x03ef, 0x03cf, 0x03af,
  0x038f, 0x036f, 0x034f, 0x032f, 0x030f, 0x02ef, 0x02cf, 0x02af, 0x028f,
  0x026f, 0x024f, 0x022f, 0x020f, 0x01ef, 0x01cf, 0x01af, 0x018f, 0x016f,
  0x014f, 0x012f, 0x010f, 0x00ef, 0x00cf, 0x00af, 0x008f, 0x006f, 0x004f,
  0x002f, 0x000f, 0x000f, 0x000f, 0x000f, 0x080f, 0x080f, 0x100f, 0x100f,
  0x180f, 0x180f, 0x200f, 0x200f, 0x280f, 0x280f, 0x300f, 0x300f, 0x380f,
  0x380f, 0x400f, 0x400f, 0x480f, 0x480f, 0x500f, 0x500f, 0x580f, 0x580f,
  0x600f, 0x600f, 0x680f, 0x680f, 0x700f, 0x700f, 0x780f, 0x780f, 0x780f,
  0x780e, 0x780e, 0x780d, 0x780d, 0x780c, 0x780c, 0x780b, 0x780b, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a, 0x780a,
  0x780a, 0x780a, 0x780a, 0x780a
};


void setup()
{
  char puffer[10];

	DIS_ST7735_displayInit();
  DIS_ST7735_setRotation(ROTATE_DEGREE_270);
	DIS_ST7735_fillScreen(ST77XX_BLACK);
	DIS_ST7735_fillRect(0, 0, 160, 21, 0x1326);
	DIS_ST7735_drawRect(0, 0, 160, 21, ST77XX_YELLOW);

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

	DIS_ST7735_fillRect(0, 27, 16+3, 4+3, 0x000c);
	DIS_ST7735_drawRect(0, 27, 16+3, 4+3, 0x7bef);
	DIS_ST7735_fillRect(0, 40, 16*ZOOM+3, 4*ZOOM+3, 0x000c);
	DIS_ST7735_drawRect(0, 40, 16*ZOOM+3, 4*ZOOM+3, 0x7bef);
	DIS_ST7735_fillRect(0, 75, 15*ZOOM+3, 3*ZOOM+3, 0x000c);
	DIS_ST7735_drawRect(0, 75, 15*ZOOM+3, 3*ZOOM+3, 0x7bef);

	SPI_startWrite();
  DIS_ST7735_setAddrWindow(150, 25, 10, 103);
  for (uint8_t i=0; i < 103; i++)                 // 103 Farb-Schritte werden angezeigt
  {
    for (uint8_t j = 0; j < 10; ++j)
    {
      uint8_t h_i = ((uint16_t)(i * 13)) >> 3; // 0x555 / 103dec = 13
      uint16_t color = pgm_read_word(colormap + h_i); //colormap[h_i];
      uint8_t color_high = color >> 8, color_low = color;
      
      SPDR = color_high;
		  while(!(SPSR & (1 << SPIF0)));
		  SPDR = color_low;
		  while(!(SPSR & (1 << SPIF0)));
    }
  }

	SPI_endWrite();

	DIS_ST7735_setFontColor(ST77XX_WHITE);
  DIS_ST7735_setFontSize(1, 1);

  itoa (MINTEMP, puffer, 10);
	DIS_ST7735_setCursor(130, 120);
  DIS_ST7735_drawString(puffer);

  itoa (MAXTEMP, puffer, 10);
	DIS_ST7735_setCursor(130, 25);
  DIS_ST7735_drawString(puffer);

  // Zero degree line
  float gradstep = (MAXTEMP + abs(MINTEMP)) / 103.0;      // Absoultwert von Min
  uint8_t i = 25 + (uint8_t)(MAXTEMP / gradstep);
  DIS_ST7735_drawHLine(145, i, 5, ST77XX_WHITE);


  _delay_ms(5000);


	DIS_ST7735_fillRect(0, 112, 71, 17, ST77XX_BLACK); // Alte Zahl loeschen  ST77XX_YELLOW

	DIS_ST7735_setFontColor(ST77XX_YELLOW);
  DIS_ST7735_setFontSize(2, 2);
	DIS_ST7735_setCursor(0, 112);

  dtostrf (gradstep*111, 6, 1, puffer); //<---- replace gradstep by t!!!
  DIS_ST7735_drawString(puffer);

  DIS_ST7735_setFontSize(1, 1);
	DIS_ST7735_setCursor(74, 112);
  DIS_ST7735_drawChar(74, 112, '\xF8'); // "°C" ASCII 0xF8 fuer Gradzeichen
	DIS_ST7735_setCursor(80, 112);
  DIS_ST7735_drawChar(80, 112, 'C'); 

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
