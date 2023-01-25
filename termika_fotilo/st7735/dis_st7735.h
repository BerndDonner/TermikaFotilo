/*___________________________________________________________________________________________________

Title:
	DIS_ST7735.h v1.3

Description:
	Library for interfacing AVR microcontrollers with the ST7735 display driver based on the Adafruit libraries:
	https://github.com/adafruit/Adafruit-ST7735-Library
	https://github.com/adafruit/Adafruit-GFX-Library
	
	For complete details visit
	https://www.programming-electronics-diy.xyz/2022/02/library-for-st7735-tft-display-driver.html

Author:
 	Liviu Istrate
	istrateliviu24@yahoo.com
	
Donate:
	Software development takes time and effort so if you find this useful consider a small donation at:
	paypal.me/alientransducer
_____________________________________________________________________________________________________*/


/* ----------------------------- LICENSE - GNU GPL v3 -----------------------------------------------

* This license must be included in any redistribution.

* Copyright (c) 2022 Liviu Istrate, www.programming-electronics-diy.xyz (istrateliviu24@yahoo.com)

* Project URL: https://www.programming-electronics-diy.xyz/2022/02/library-for-st7735-tft-display-driver.html
* Based on the Adafruit libraries made for Arduino:
* 		https://github.com/adafruit/Adafruit-ST7735-Library
* 		https://github.com/adafruit/Adafruit-GFX-Library

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.

--------------------------------- END OF LICENSE --------------------------------------------------*/

#ifndef DIS_ST7735_H
#define DIS_ST7735_H

/*************************************************************
	INCLUDES
**************************************************************/
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "utils.h"

/*************************************************************
	USER DEFINED SETTINGS
**************************************************************/

// Arduino UNO setup:
//SCL   -> SCK   -> PB5
//SDA   -> MOSI  -> PB3
//CS    -> SS    -> PB2
//A0    -> (D/C) -> PB1
//Reset ->       -> PB0

// It is important that the SS pin is set as an output - low or high
// otherwise the SPI won't work properly. E.g if the SS pin is PB4:
// DDRB |= 1 << PB4;

// SPI0 I/O pins (MOSI and SCK)
#define SPI0_PORT								PORTB // PORT for SPI pins
#define SPI0_DDR								DDRB  // DDR for SPI pins
#define SPI0_PINS								PINB  // Holds the state of each pin on port where SPI pins are

#define SPI0_MOSI_PIN							PB3   // SDA pin
#define SPI0_SCK_PIN							PB5   // SCK pin

// TFT RESET pin (optional - comment-out if not used)
// If not used connect it to VCC
#define TFT_RESET_DDR							DDRB
#define TFT_RESET_PORT						PORTB
#define TFT_RESET_PIN							PB0

// TFT D/C (Data/Command) pin
#define TFT_DC_DDR								DDRB
#define TFT_DC_PORT								PORTB
#define TFT_DC_PIN								PB1

// TFT CS (Chip Select) - can be SPI SS or any other pin
#define TFT_CS_DDR								DDRB
#define TFT_CS_PORT								PORTB
#define TFT_CS_PIN								PB2

// TFT BL (Back Light) pin (optional - comment-out if not used)
//#define TFT_BL_DDR							DDRx
//#define TFT_BL_PORT							PORTx
//#define TFT_BL_PIN							Pxn

#define DISPLAY_WIDTH							128
#define DISPLAY_HEIGHT						160

// Some display needs this offset during initialization
// If a pixel written at 0,0 location is not at 0,0 set
// x and y offsets to 0, 0 or 2, 1 depending on the display
#define X_OFFSET								0 // default: 2
#define Y_OFFSET								0 // default: 1

// RGB color filter panel - RGB or BGR (MADCTL)
// If red is displayed as blue set this value as 0x00 otherwise 0x08
#define ST7735_RGB_FILTER						0x08


// Some ready-made 16-bit ('565') color settings:
// To add more colors by name visit:
// https://htmlcolorcodes.com/color-names/
// then use the RGB565.exe to convert the color to 565 format
// https://github.com/lgaland/rgb-converter
#define ST77XX_BLACK					0x0000
#define ST77XX_WHITE 					0xFFFF
#define ST77XX_RED 					0xF800
#define ST77XX_GREEN 					0x07E0
#define ST77XX_BLUE 					0x001F
#define ST77XX_CYAN 					0x07FF
#define ST77XX_MAGENTA 					0xF81F
#define ST77XX_YELLOW 					0xFFE0
#define ST77XX_ORANGE 					0xFC00
#define ST77XX_GREY					0x8410
#define ST77XX_LIGHT_GREY				0xD69A
#define ST77XX_DIM_GREY					0x6B4D

/*************************************************************
	END OF USER DEFINED SETTINGS
**************************************************************/


/*************************************************************
	SYSTEM DEFINES CONTINUED
**************************************************************/
#define SPIF0   7
#define SPIE0   7
#define SPR00   0
#define SPR01   1
#define MSTR0   4
#define SPE0    6

#define TEXT_ALIGN_LEFT			0
#define TEXT_ALIGN_RIGHT		1
#define TEXT_ALIGN_CENTER		2

// These control the screen rotation and RGB filter (RGB or BGR)
#define ST7735_MADCTL_MV		0x20
#define ST7735_MADCTL_MX		0x40
#define ST7735_MADCTL_MY		0x80
#define ST7735_MADCTL_REG		(ST7735_RGB_FILTER | ST7735_MADCTL_MX | ST7735_MADCTL_MY)

#define ROTATE_DEGREE_0		0	// no rotation
#define ROTATE_DEGREE_90	1	// rotate 90 degree CCW
#define ROTATE_DEGREE_180	2	// rotate 180 degree CCW
#define ROTATE_DEGREE_270	3	// rotate 270 degree CCW

#define ST_CMD_DELAY 		0x80 // special signifier for command lists

// TFT HEX Commands
#define ST7735_NOP 			0x00
#define ST7735_SWRESET 		0x01
#define ST7735_RDDID 		0x04
#define ST7735_RDDST 		0x09
#define ST7735_RDDMADCTL	0x0B
#define ST7735_RDDCOLMOD	0x0C
#define ST7735_RDID1 		0xDA
#define ST7735_RDID2 		0xDB
#define ST7735_RDID3 		0xDC
#define ST7735_RDID4 		0xDD
#define ST7735_SLPIN  		0x10
#define ST7735_SLPOUT  		0x11
#define ST7735_PTLON  		0x12
#define ST7735_NORON  		0x13
#define ST7735_INVOFF 		0x20
#define ST7735_INVON 		0x21
#define ST7735_DISPOFF 		0x28
#define ST7735_DISPON 		0x29
#define ST7735_CASET 		0x2A
#define ST7735_RASET 		0x2B
#define ST7735_RAMWR 		0x2C
#define ST7735_RAMRD 		0x2E
#define ST7735_COLMOD 		0x3A
#define ST7735_MADCTL 		0x36
#define ST7735_FRMCTR1 		0xB1
#define ST7735_FRMCTR2 		0xB2
#define ST7735_FRMCTR3 		0xB3
#define ST7735_INVCTR 		0xB4
#define ST7735_DISSET5 		0xB6
#define ST7735_PWCTR1 		0xC0
#define ST7735_PWCTR2 		0xC1
#define ST7735_PWCTR3 		0xC2
#define ST7735_PWCTR4 		0xC3
#define ST7735_PWCTR5 		0xC4
#define ST7735_VMCTR1 		0xC5
#define ST7735_PWCTR6 		0xFC
#define ST7735_GMCTRP1 		0xE0
#define ST7735_GMCTRN1 		0xE1
#define ST7735_TEOFF 		0x34
#define ST7735_TEON 		0x35
#define ST7735_IDMOFF 		0x38
#define ST7735_IDMON 		0x39

// Used as a parameter for DIS_ST7735_drawCircleHelper()
#define ST77XX_TOP_LEFT_CORNER			0x1
#define ST77XX_TOP_RIGHT_CORNER			0x2
#define ST77XX_BOTTOM_RIGHT_CORNER		0x4
#define ST77XX_BOTTOM_LEFT_CORNER		0x8
#define ST77XX_ALL_CORNERS				0xF

/*************************************************************
	MACRO FUNCTIONS
**************************************************************/
#define SPI_DC_HIGH_DATA()						TFT_DC_PORT |= (1 << TFT_DC_PIN)
#define SPI_DC_LOW_CMD()						TFT_DC_PORT &= ~(1 << TFT_DC_PIN)

#define SPI_TFT_CS_HIGH()						TFT_CS_PORT |= (1 << TFT_CS_PIN)
#define SPI_TFT_CS_LOW()						TFT_CS_PORT &= ~(1 << TFT_CS_PIN)

#define DIS_ST7735_readManufacturerID()			DIS_ST7735_readcommand8(ST7735_RDDID, 0) // 124
#define DIS_ST7735_readVersionID()				DIS_ST7735_readcommand8(ST7735_RDDID, 1) // 137
#define DIS_ST7735_readDriverID()				DIS_ST7735_readcommand8(ST7735_RDDID, 2) // 240

#define DIS_ST7735_backlightOn()				TFT_BL_PORT |= (1 << TFT_BL_PIN)
#define DIS_ST7735_backlightOff()				TFT_BL_PORT &= ~(1 << TFT_BL_PIN)

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b){                    \
    int16_t t = a;                              \
    a = b;                                      \
    b = t;                                      \
}
#endif


/*************************************************************
	GLOBALS
**************************************************************/
static int16_t WIDTH;        		// Display width - never changes
static int16_t HEIGHT;       		// Display height - never changes
static int16_t _width;       		// Display width as modified by current rotation
static int16_t _height;      		// Display height as modified by current rotation
static uint8_t _xstart;		 		// Some displays need this offset
static uint8_t _ystart;		 		// Some displays need this offset
static int16_t cursor_x = 0;   		// cursor's x location
static int16_t cursor_y = 0;   		// cursor's y location
static uint16_t textcolor = ST77XX_BLACK; // 16-bit text color
static uint8_t textsize_x = 1;   	// Desired magnification in X-axis of text
static uint8_t textsize_y = 1;   	// Desired magnification in Y-axis of text
static uint8_t lineThickness = 1;
static uint8_t textLimitLeft;
static uint8_t textLimitRight;
static uint8_t textLimitTop;
static uint8_t textLimitBottom;
static uint8_t rotation;     		// Display rotation (0 through 3)
static uint16_t bgColor = 1;
static uint16_t string_offset = 0;
static bool set_font_bold = false;

// Font data stored per GLYPH
typedef struct{
	uint16_t bitmapOffset; // Pointer into GFXfont->bitmap
	uint8_t width;         // Bitmap dimensions in pixels
	uint8_t height;        // Bitmap dimensions in pixels
	uint8_t xAdvance;      // Distance to advance cursor (x axis)
	int8_t xOffset;        // X dist from cursor position to UL corner
	int8_t yOffset;        // Y dist from cursor position to UL corner
} GFXglyph;

// Data stored for FONT AS A WHOLE
typedef struct{
	uint8_t *bitmap;  // Glyph bitmaps, concatenated
	GFXglyph *glyph;  // Glyph array
	uint16_t first;   // ASCII extents (first char)
	uint16_t last;    // ASCII extents (last char)
	uint8_t yAdvance; // Newline distance (y axis)
} GFXfont;

static const GFXfont *gfxFont;


// Commands for screen initialization
// This section of code is from the Adafruit_ST7735 library
static const uint8_t PROGMEM Icmd[] = {
	21,									// 21 commands in list:
    ST7735_SWRESET,   ST_CMD_DELAY,		//  1: Software reset, 0 args, w/delay
	150,								//     150 ms delay
    ST7735_SLPOUT,    ST_CMD_DELAY,		//  2: Out of sleep mode, 0 args, w/delay
	150,								//     150 ms delay
	ST7735_FRMCTR1, 3,					//  3: Frame rate control, 3 args:
	0x00,								//     fastest refresh
	0x06,								//     6 lines front porch
	0x03,								//     3 lines back porch
	ST7735_FRMCTR2, 3,					//  4: Framerate ctrl - idle mode, 3 args:
	0x01, 0x2C, 0x2D,					//     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3, 6,					//  5: Framerate - partial mode, 6 args:
	0x01, 0x2C, 0x2D,					//     Dot inversion mode
	0x01, 0x2C, 0x2D,					//     Line inversion mode
    ST7735_INVCTR,  1,					//  6: Display inversion ctrl, 1 arg:
	0x07,								//     No inversion
    ST7735_PWCTR1,  3,					//  7: Power control, 3 args, no delay:
	0xA2,
	0x02,								//     -4.6V
	0x84,								//     AUTO mode
    ST7735_PWCTR2,  1,					//  8: Power control, 1 arg, no delay:
	0xC5,								//     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
    ST7735_PWCTR3,  2,					//  9: Power control, 2 args, no delay:
	0x0A,								//     Opamp current small
	0x00,								//     Boost frequency
    ST7735_PWCTR4,  2,					// 10: Power control, 2 args, no delay:
	0x8A,								//     BCLK/2,
	0x2A,								//     opamp current small & medium low
    ST7735_PWCTR5,  2,					// 11: Power control, 2 args, no delay:
	0x8A, 0xEE,
    ST7735_VMCTR1,  1,					// 12: Power control, 1 arg, no delay:
	0x0E,
    ST7735_INVOFF,  0,					// 13: Don't invert display, no args
    ST7735_MADCTL,  1,					// 14: Mem access ctl (directions), 1 arg:
	ST7735_MADCTL_REG,					//     row/col addr, bottom-top refresh
    ST7735_COLMOD,  1,					// 15: set color mode, 1 arg, no delay:
	0x05,								//     16-bit color
	ST7735_CASET,   4,					// 16: Column addr set, 4 args, no delay:
	0x00, 0x00 + X_OFFSET,				//     XSTART = 0 + offset
	0x00, 0x7F + X_OFFSET,				//     XEND = 127 + offset
    ST7735_RASET,   4,					// 17: Row addr set, 4 args, no delay:
	0x00, 0x00 + Y_OFFSET,				//     XSTART = 0 + offset
	0x00, 0x9F + Y_OFFSET,				//     XEND = 159 + offset
	ST7735_GMCTRP1, 16,					// 18: Gamma Adjustments (pos. polarity), 16 args + delay:
	0x02, 0x1c, 0x07, 0x12,				//     (Not entirely necessary, but provides
	0x37, 0x32, 0x29, 0x2d,				//      accurate colors)
	0x29, 0x25, 0x2B, 0x39,
	0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16,					// 19: Gamma Adjustments (neg. polarity), 16 args + delay:
	0x03, 0x1d, 0x07, 0x06,				//     (Not entirely necessary, but provides
	0x2E, 0x2C, 0x29, 0x2D,				//      accurate colors)
	0x2E, 0x2E, 0x37, 0x3F,
	0x00, 0x00, 0x02, 0x10,
    ST7735_NORON,     ST_CMD_DELAY,		// 20: Normal display on, no args, w/delay
	10,									//     10 ms delay
    ST7735_DISPON,    ST_CMD_DELAY,		// 21: Main screen turn on, no args, w/delay
	100									//     100 ms delay
};



/*************************************************************
	FUNCTION PROTOTYPES
**************************************************************/
void DIS_ST7735_displayInit(void);
uint16_t DIS_ST7735_getStringOffset(void);
uint8_t DIS_ST7735_drawString(const char *c);
uint8_t DIS_ST7735_drawString(const __FlashStringHelper *flash);
uint8_t DIS_ST7735_drawChar(int16_t x, int16_t y, unsigned char c);
void DIS_ST7735_drawInt(INT_SIZE number, int8_t nrOfDigits);
void DIS_ST7735_drawFloat(float float_number, int8_t nrOfDigits, uint8_t nrOfDecimals);
void DIS_ST7735_setFont(const GFXfont *f);
void DIS_ST7735_setFontBold(bool bold);
void DIS_ST7735_setFontSize(uint8_t size_x, uint8_t size_y);
void DIS_ST7735_setFontColor(uint16_t color);
void DIS_ST7735_setBackgroundColor(uint16_t color);
void DIS_ST7735_textAlign(uint8_t align, uint8_t nrOfPixels);
void DIS_ST7735_setTextBoundaries(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom);
void DIS_ST7735_setLineThickness(uint8_t thickness);
void DIS_ST7735_drawMonochromeBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, uint8_t tr);
//void DIS_ST7735_drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h);
void DIS_ST7735_drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
void DIS_ST7735_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void DIS_ST7735_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void DIS_ST7735_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
void DIS_ST7735_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void DIS_ST7735_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void DIS_ST7735_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void DIS_ST7735_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color);
void DIS_ST7735_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void DIS_ST7735_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color);
void DIS_ST7735_fillScreen(uint16_t color);
void DIS_ST7735_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void DIS_ST7735_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void DIS_ST7735_setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DIS_ST7735_setCursor(uint8_t x, uint8_t y);
void DIS_ST7735_setCursorX(uint8_t x);
void DIS_ST7735_setCursorY(uint8_t y);
void DIS_ST7735_setRotation(uint8_t x);
void DIS_ST7735_writePixel(int16_t x, int16_t y, uint16_t color);
void DIS_ST7735_drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void DIS_ST7735_drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void DIS_ST7735_gotoNextLine(uint8_t x);
uint16_t rgbConvert24to16(uint32_t rgb24);
uint8_t DIS_ST7735_getDisplayWidth(void);
uint8_t DIS_ST7735_getDisplayHeight(void);
uint8_t DIS_ST7735_getRowCount(void);
uint8_t DIS_ST7735_getColumnCount(void);
uint8_t DIS_ST7735_getCursorX(void);
uint8_t DIS_ST7735_getCursorY(void);
uint8_t DIS_ST7735_getLineHeight(void);
uint8_t DIS_ST7735_getLineYStart(uint8_t lineNr);
uint8_t DIS_ST7735_getTextWidth(const char *c);
uint8_t DIS_ST7735_readcommand8(uint8_t commandByte, int8_t index);

void DIS_ST7735_invertDisplay(bool i);
void DIS_ST7735_enableSleep(bool enable);
void DIS_ST7735_enableTearing(bool enable);
void DIS_ST7735_enableDisplay(bool enable);
void DIS_ST7735_idleMode(bool enable);

static void DIS_ST7735_sendBulkCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes);
static void DIS_ST7735_writeData(uint8_t data);
static void DIS_ST7735_writeCommand(uint8_t cmd);
static void DIS_ST7735_sendCommand(uint8_t cmd);

static void SPI_startWrite(void);
static void SPI_endWrite(void);
static void SPI_init(void);
static void SPI_transmit(uint32_t data, uint8_t data_size);

/*************************************************************
	FUNCTIONS
**************************************************************/


/**************************************************************************
	@brief  Reads and issues a series of LCD commands stored in PROGMEM byte array.
			Setup the SPI.
			
	@param  none
***************************************************************************/
void DIS_ST7735_displayInit(void){
	uint8_t numCommands = 0;
	uint8_t cmd, numArgs, ms = 0;
	const uint8_t *addr = Icmd;
  
/* I/O */
	// Reset pin - Output High
	#ifdef TFT_RESET_DDR
	TFT_RESET_DDR |= (1 << TFT_RESET_PIN);
	#endif
	
	// D/C pin - Output High
	TFT_DC_PORT &= ~(1 << TFT_DC_PIN);
	TFT_DC_DDR |= (1 << TFT_DC_PIN);
	
	// CS pin - Output High
	TFT_CS_PORT &= ~(1 << TFT_CS_PIN);
	TFT_CS_DDR |= (1 << TFT_CS_PIN);
	
	// BL pin - Output High
	#ifdef TFT_BL_DDR
	TFT_BL_PORT |= (1 << TFT_BL_PIN);
	TFT_BL_DDR |= (1 << TFT_BL_PIN);
	#endif
	
/* SPI */
	SPI_init();
	
/* TFT Display Settings */
	WIDTH = _width = DISPLAY_WIDTH;
	HEIGHT = _height = DISPLAY_HEIGHT;
	
	_xstart = X_OFFSET;
	_ystart = Y_OFFSET;
	
/* TFT Init */
	#ifdef TFT_RESET_DDR
	TFT_RESET_PORT &= ~(1 << TFT_RESET_PIN);
	_delay_ms(1);
	TFT_RESET_PORT |= (1 << TFT_RESET_PIN);
	_delay_ms(200);
	#endif
	
	numCommands = pgm_read_byte(addr++);	// Number of commands to follow
	
	while(numCommands--){					// For each command...
		cmd = pgm_read_byte(addr++);		// Read command
		numArgs = pgm_read_byte(addr++);	// Number of args to follow
		ms = numArgs & ST_CMD_DELAY;		// If MSB is set, delay follows args
		numArgs &= ~(ST_CMD_DELAY);			// Mask out delay bit
		DIS_ST7735_sendBulkCommand(cmd, addr, numArgs);
		
		if(ms){
			ms = pgm_read_byte(addr++);	// Read post-command delay time (ms)
			while(ms--){
				_delay_ms(1);
			}
		}
		
		addr += numArgs;
	}
}



/************************************************************************************************
	@brief	When printing a string using DIS_ST7735_drawChar() and the text overflows the screen
			on the Y axis the DIS_ST7735_drawChar() will return 1. Use the return value of this 
			function to offset the string pointer to print from the start of the screen the
			continuation of the string. The return value represents the number of characters 
			printed before the overflow occurs.
*************************************************************************************************/
uint16_t DIS_ST7735_getStringOffset(void){
	uint16_t buff = string_offset;
	string_offset = 0;
	return buff;
}


/************************************************************************************************
	@brief	Draw a string of characters starting from the current cursor position.
			It returns 1 if screen overflow or 0 otherwise.
	
	@param	c		The 8-bit characters
*************************************************************************************************/
uint8_t DIS_ST7735_drawString(const char *c)
{
	while(*c > 0)
  {
		if(*c == '\n') DIS_ST7735_gotoNextLine(textLimitLeft);
		else if(DIS_ST7735_drawChar(cursor_x, cursor_y, *c) == 1) return 1;
		c++;
		string_offset++;
	}
	return 0;
}
/************************************************************************************************
	@brief	Draw a string of characters starting from the current cursor position.
			It returns 1 if screen overflow or 0 otherwise.
	
	@param	c		The 8-bit characters
*************************************************************************************************/
uint8_t DIS_ST7735_drawString(const __FlashStringHelper *flash){
  PGM_P p = reinterpret_cast<PGM_P>(flash);
  while (1) {
    unsigned char c = pgm_read_byte(p++);
    if (c == 0) return 0;
		if(c == '\n') DIS_ST7735_gotoNextLine(textLimitLeft);
		else if(DIS_ST7735_drawChar(cursor_x, cursor_y, c) == 1) return 1;
		string_offset++;
	}	
}


/************************************************************************************************
	@brief	Draw a single character. It returns 1 if on screen overflow or 0 otherwise
	
	@param	x		Bottom left corner x coordinate
	@param	y		Bottom left corner y coordinate
	@param	c		The 8-bit font-indexed character (likely ascii)
*************************************************************************************************/
uint8_t DIS_ST7735_drawChar(int16_t x, int16_t y, unsigned char c){

	if(gfxFont == 0) return 2;
	
	uint8_t first = pgm_read_byte(&gfxFont->first);
	uint8_t last = pgm_read_byte(&gfxFont->last);
	
	// Char present in this font?

	if(!((c >= first) && (c <= last))) c = '_';
	
	// Adapt ASCII number to array index
	c -= first;
	
	// Get the glyph and bitmap address
	GFXglyph *glyph = &(((GFXglyph *)pgm_read_word(&gfxFont->glyph))[c]);
	uint8_t *bitmap = (uint8_t *)pgm_read_word(&gfxFont->bitmap);

	// Read the bitmap offset
	uint16_t bo = pgm_read_word(&glyph->bitmapOffset);
	
	// Width and height of the glyph
	uint8_t w = pgm_read_byte(&glyph->width), h = pgm_read_byte(&glyph->height);
	
	// Distance to advance cursor (x axis)
	uint8_t xAdvance = pgm_read_byte(&glyph->xAdvance);
	// New line distance (y axis)
	uint8_t yAdvance = pgm_read_byte(&gfxFont->yAdvance);
	
	// X and Y offset
	int8_t xo = pgm_read_byte(&glyph->xOffset), yo = pgm_read_byte(&glyph->yOffset);
	
	uint8_t xx, yy, bits = 0, bit = 0;
	int16_t xo16 = x + (w * textsize_x);
	int16_t yo16 = 0;
	
	// Not off left
	if(x < textLimitLeft) x = cursor_x = textLimitLeft;
	
	// Not off right
	if((xo16 > _width - 1 - textLimitRight)){
		x = cursor_x = textLimitLeft;
		y = cursor_y = cursor_y + ((uint16_t)yAdvance * textsize_y);
	}
	
	// Not off bottom
	if(cursor_y > _height - 1 - textLimitBottom){
		x = cursor_x = textLimitLeft;
		//y = cursor_y = (uint16_t)yAdvance * textsize_y + textLimitTop;
		return 1;
	}

	// Use 16-bit variables if char size is greater than 1
	if((textsize_x > 1) || (textsize_y > 1)){
		xo16 = xo;
		yo16 = yo;
	}
	
	// Clear one character area
	if(bgColor != 1){
		DIS_ST7735_fillRect(cursor_x, cursor_y - yAdvance + 6, xAdvance + 5, yAdvance - 3, bgColor);
	}
	
	SPI_startWrite();
	
	for(yy = 0; yy < h; yy++){
		for(xx = 0; xx < w; xx++){
			
			if(!(bit++ & 7)){
				bits = pgm_read_byte(&bitmap[bo++]);
			}
			
			if(bits & 0x80){
				if(textsize_x == 1 && textsize_y == 1){
					DIS_ST7735_writePixel(x + xo + xx, y + yo + yy, textcolor);
					
					if(set_font_bold){
						DIS_ST7735_writePixel(x + xo + xx + 1, y + yo + yy, textcolor);
					}
				}else{
					DIS_ST7735_fillRect(x + (xo16 + xx) * textsize_x, y + (yo16 + yy) * textsize_y, textsize_x, textsize_y, textcolor);
				}
			}

			bits <<= 1;
		}
    }
	
    SPI_endWrite();
	
	// Advance the cursor on x axis
	cursor_x += (xAdvance + 1) * textsize_x;
	
	return 0;
}



/************************************************************************************************
	@brief	Draw an integer number to display
	
    @param	number			The number
    @param	nrOfDigits		Total number of digits desired. If the number to be displayed 
							has less digits than "nrOfDigits" then it will be padded with zeros.
							Useful for maintaining the GUI layout and for displaying floats.
							
							E.g:	
							uint16_t ADC_results = 120;
							DIS_ST7735_drawInt(ADC_results, 5);
							will display "00120"
							
							DIS_ST7735_drawInt(ADC_results, 3);
							will display "120"
*************************************************************************************************/
void DIS_ST7735_drawInt(INT_SIZE number, int8_t nrOfDigits){
	char string[MAX_NR_OF_DIGITS + 1] = {0};
		
	STRING_itoa(number, string, nrOfDigits);
	DIS_ST7735_drawString(string);
}



/************************************************************************************************
	@brief	Draw a float number to display
	
    @param	float_number	The float number
    @param	nrOfDigits		Total number of digits desired. If the number to be displayed 
							has less digits than "nrOfDigits" then it will be padded with zeros.
							Useful for maintaining the GUI layout.
							
							E.g:	
							uint16_t temp = 12.5;
							DIS_ST7735_drawFloat(temp, 5);
							will display "00012.5"
							
							DIS_ST7735_drawFloat(temp, 2);
							will display "12.5"
	@param	nrOfDecimals	Number of digits after the point
*************************************************************************************************/
void DIS_ST7735_drawFloat(float float_number, int8_t nrOfDigits, uint8_t nrOfDecimals){
	char string_integer[MAX_NR_OF_DIGITS + 1];
	char string_float[MAX_NR_OF_DIGITS + 1];
	
	STRING_ftoa(float_number, string_integer, string_float, nrOfDigits, nrOfDecimals);

	if(float_number < 0) DIS_ST7735_drawChar(cursor_x, cursor_y, '-');
	DIS_ST7735_drawString(string_integer);
	DIS_ST7735_drawChar(cursor_x, cursor_y, '.');
	DIS_ST7735_drawString(string_float);
}



/************************************************************************************************
	@brief	Set the font family
	
    @param	f	The GFXfont object
*************************************************************************************************/
void DIS_ST7735_setFont(const GFXfont *f){
	gfxFont = (GFXfont *)f;
}


/************************************************************************************************
	@brief	Cheap way to bold a font
	
	@param	bold	true or false
*************************************************************************************************/
void DIS_ST7735_setFontBold(bool bold){
	set_font_bold = bold;
}



/************************************************************************************************
	@brief	Set the font size
	
	@param	size_x	Font magnification level in X-axis, 1 is 'original' size
	@param	size_y	Font magnification level in Y-axis, 1 is 'original' size
*************************************************************************************************/
void DIS_ST7735_setFontSize(uint8_t size_x, uint8_t size_y){
	textsize_x = size_x;
	textsize_y = size_y;
}



/************************************************************************************************
	@brief	Set the font color
	
	@param	color			16-bit 5-6-5 Color to draw character with
*************************************************************************************************/
void DIS_ST7735_setFontColor(uint16_t color){
	textcolor = color;
}



/************************************************************************************************
	@brief	Set background color before drawing text or numbers to erase previous character
	
	@param	color			16-bit 5-6-5 Color to erase the character with
*************************************************************************************************/
void DIS_ST7735_setBackgroundColor(uint16_t color){
	bgColor = color;
}



/************************************************************************************************
	@brief	Text alignment
	
	@param	align			Can be TEXT_ALIGN_LEFT, TEXT_ALIGN_RIGHT, TEXT_ALIGN_CENTER
	@param nrOfPixels		Number of pixels that the string of characters takes and can be obtained 
							using DIS_ST7735_getTextWidth(string)
*************************************************************************************************/
void DIS_ST7735_textAlign(uint8_t align, uint8_t nrOfPixels){
	if(align == TEXT_ALIGN_RIGHT){
		cursor_x = _width - 1 - nrOfPixels;
	}else if(align == TEXT_ALIGN_CENTER){
		cursor_x = ((_width - 1) / 2) - (nrOfPixels / 2);
	}
}


/************************************************************************************************
	@brief	Set text boundaries. It's like an invisible rectangle used to frame the text
	
    @param	left	Text will start from this position (x axis)
    @param	right	Text will end at this position (x axis)
    @param	top		Text will not be printed above this y coordinate
    @param	bottom	Text will not be printed beyond this y coordinate
*************************************************************************************************/
void DIS_ST7735_setTextBoundaries(uint8_t left, uint8_t right, uint8_t top, uint8_t bottom){
	textLimitLeft = left;
	textLimitRight = right;
	textLimitTop = top;
	textLimitBottom = bottom;
}




/************************************************************************************************
	@brief	Sets the line thickness for the primitive shapes
	
	@param	thickness	The line thickness is pixels. Default is 1.
*************************************************************************************************/
void DIS_ST7735_setLineThickness(uint8_t thickness){
	lineThickness = thickness;
}



/************************************************************************************************
	@brief	Draw a PROGMEM-resident 1-bit image at the specified (x,y) position, using the 
			specified foreground (for set bits) and background (unset bits) colors.
			Used to draw small monochrome (single color) bitmaps, good for sprites and other 
			mini-animations or icons.
			XBitMap Files (*.xbm), exported from GIMP can also be used.
			Usage: Export from GIMP to *.xbm, rename *.xbm to *.c and open in editor.
			C Array can be directly used with this function.
			
	@param	x			Top left corner x coordinate
	@param	y			Top left corner y coordinate
	@param	bitmap		byte array with monochrome bitmap
	@param	w			Width of bitmap in pixels
	@param	h			Height of bitmap in pixels
	@param	color		16-bit 5-6-5 color to draw with
	@param  bg			16-bit 5-6-5 color to draw background with. If tr is 1 this param will be ignored
	@param  tr			if 1 the background will be transparent. If tr is 0 the background color
						is defined by the bg param
*************************************************************************************************/
void DIS_ST7735_drawMonochromeBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color, uint16_t bg, uint8_t tr){
	int16_t byteWidth = (w + 7) / 8; // Bitmap scan line pad = whole byte
	uint8_t byte = 0;

	SPI_startWrite();
	
	for(int16_t j = 0; j < h; j++, y++){
		for(int16_t i = 0; i < w; i++){
			if(i & 7) // 0 or 8 & 7 => 0
				//byte <<= 1; // shift the byte left
				byte >>= 1; // shift the byte right
			else
				// Read a byte
				byte = pgm_read_byte(&bitmap[j * byteWidth + i / 8]);
			
			// Check the MSB if 1
			//if(byte & 0x80)	DIS_ST7735_writePixel(x + i, y, color);
			if(byte & 0x01)	DIS_ST7735_writePixel(x + i, y, color);
			else if(tr == 0) DIS_ST7735_writePixel(x + i, y, bg);
		}
	}
	
	SPI_endWrite();
}



/************************************************************************************************
   @brief	Draw a PROGMEM-resident 8-bit image (grayscale) at the specified (x,y) position. 
			Specifically for 8-bit display devices such as IS31FL3731; no color reduction/expansion
			is performed so it will not work on color screens without the proper color conversion.
   
    @param    x			Top left corner x coordinate
    @param    y			Top left corner y coordinate
    @param    bitmap	byte array with grayscale bitmap
    @param    w			Width of bitmap in pixels
    @param    h			Height of bitmap in pixels
*************************************************************************************************/
/*
void DIS_ST7735_drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h){
	SPI_startWrite();
	
	for(int16_t j = 0; j < h; j++, y++){
		for(int16_t i = 0; i < w; i++){
			DIS_ST7735_writePixel(x + i, y, (uint8_t)pgm_read_byte(&bitmap[j * w + i]));
		}
	}
	
	SPI_endWrite();
}
*/



/************************************************************************************************
	@brief		Draw a PROGMEM-resident 16-bit image (RGB 5/6/5) at the specified (x,y) position.
				For 16-bit display devices; no color reduction performed.
   
    @param    x			Top left corner x coordinate
    @param    y			Top left corner y coordinate
    @param    bitmap	byte array with 16-bit color bitmap
    @param    w			Width of bitmap in pixels
    @param    h			Height of bitmap in pixels
*************************************************************************************************/
void DIS_ST7735_drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h){
	SPI_startWrite();
	
	for(int16_t j = 0; j < h; j++, y++){
		for(int16_t i = 0; i < w; i++){
			DIS_ST7735_writePixel(x + i, y, pgm_read_word(&bitmap[j * w + i]));
		}
	}
	
	SPI_endWrite();
}



/************************************************************************************************
	@brief	Fill the screen completely with one color
	
	@param	color	16-bit 5-6-5 color to fill with
*************************************************************************************************/
void DIS_ST7735_fillScreen(uint16_t color){
	DIS_ST7735_fillRect(0, 0, _width, _height, color);
}


/************************************************************************************************
	@brief	Fill a rectangle completely with one color
	
	@param	x			Top left corner x coordinate
	@param	y			Top left corner y coordinate
	@param	w			Width in pixels
	@param	h			Height in pixels
	@param	color		16-bit 5-6-5 color to fill with
*************************************************************************************************/
void DIS_ST7735_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	uint8_t color_high = color >> 8, color_low = color;
	uint16_t num_pixels = (w * h);

	SPI_startWrite();
	DIS_ST7735_setAddrWindow(x, y, w, h);

	while(num_pixels--){
		SPDR = color_high;
		while(!(SPSR & (1 << SPIF0)));
		SPDR = color_low;
		while(!(SPSR & (1 << SPIF0)));
	}
	
	SPI_endWrite();
}



/************************************************************************************************
	@brief	Draw a rectangle with no fill color
	
	@param	x			Top left corner x coordinate
	@param	y			Top left corner y coordinate
	@param	w			Width in pixels
	@param	h			Height in pixels
	@param	color		16-bit 5-6-5 color to fill with
*************************************************************************************************/
void DIS_ST7735_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	DIS_ST7735_drawHLine(x, y, w, color);
	DIS_ST7735_drawHLine(x, y + h - lineThickness, w, color);
	DIS_ST7735_drawVLine(x, y, h, color);
	DIS_ST7735_drawVLine(x + w - lineThickness, y, h, color);
}



/************************************************************************************************
	@brief	Draw a rounded rectangle with no fill color
	
	@param	x		Top left corner x coordinate
	@param	y		Top left corner y coordinate
	@param	w		Width in pixels
	@param	h		Height in pixels
	@param	r		Radius of corner rounding
	@param	color	16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color){
	int16_t max_radius = ((w < h) ? w : h) / 2; // 1/2 minor axis
	if(r > max_radius)	r = max_radius;
	
	// Smarter version
	SPI_startWrite();
	DIS_ST7735_drawHLine(x + r, y, w - 2 * r, color);         // Top
	DIS_ST7735_drawHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
	DIS_ST7735_drawVLine(x, y + r, h - 2 * r, color);         // Left
	DIS_ST7735_drawVLine(x + w - 1, y + r, h - 2 * r, color); // Right
	
	// Draw four corners
	DIS_ST7735_drawCircleHelper(x + r, y + r, r, 1, color);
	DIS_ST7735_drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
	DIS_ST7735_drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
	DIS_ST7735_drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
	SPI_endWrite();
}



/************************************************************************************************
    @brief Quarter-circle drawer, used to do circles and round rects
	
    @param x0			Center-point x coordinate
    @param y0			Center-point y coordinate
    @param r			Radius of circle
    @param cornername	Mask bit #1 or bit #2 to indicate which quarters of
						the circle we're doing
    @param color		16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color){
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	
	SPI_startWrite();

	while(x < y){
		if(f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		if(cornername & 0x4){
			DIS_ST7735_writePixel(x0 + x, y0 + y, color);
			DIS_ST7735_writePixel(x0 + y, y0 + x, color);
		}
		
		if(cornername & 0x2){
			DIS_ST7735_writePixel(x0 + x, y0 - y, color);
			DIS_ST7735_writePixel(x0 + y, y0 - x, color);
		}
		
		if(cornername & 0x8){
			DIS_ST7735_writePixel(x0 - y, y0 + x, color);
			DIS_ST7735_writePixel(x0 - x, y0 + y, color);
		}
		
		if(cornername & 0x1){
			DIS_ST7735_writePixel(x0 - y, y0 - x, color);
			DIS_ST7735_writePixel(x0 - x, y0 - y, color);
		}
	}
	
	SPI_endWrite();
}



/************************************************************************************************
    @brief  Quarter-circle drawer with fill, used for circles and round rects
	
    @param  x0       Center-point x coordinate
    @param  y0       Center-point y coordinate
    @param  r        Radius of circle
    @param  corners  Mask bits indicating which quarters we're doing
    @param  delta    Offset from center-point, used for round-rects
    @param  color    16-bit 5-6-5 Color to fill with
*************************************************************************************************/
void DIS_ST7735_fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t corners, int16_t delta, uint16_t color){

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	int16_t px = x;
	int16_t py = y;

	delta++; // Avoid some +1's in the loop

	while(x < y){
		if(f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		
		x++;
		ddF_x += 2;
		f += ddF_x;
		
		// These checks avoid double-drawing certain lines, important
		// for the SSD1306 library which has an INVERT drawing mode
		if(x < (y + 1)){
			if(corners & 1)	DIS_ST7735_drawVLine(x0 + x, y0 - y, 2 * y + delta, color);
			if(corners & 2)	DIS_ST7735_drawVLine(x0 - x, y0 - y, 2 * y + delta, color);
		}
		
		if(y != py){
			if(corners & 1)	DIS_ST7735_drawVLine(x0 + py, y0 - px, 2 * px + delta, color);
			if(corners & 2)	DIS_ST7735_drawVLine(x0 - py, y0 - px, 2 * px + delta, color);
			py = y;
		}
		
		px = x;
	}
}


/************************************************************************************************
	@brief    Draw a circle with filled color
	
	@param    x0		Center-point x coordinate
	@param    y0		Center-point y coordinate
	@param    r			Radius of circle
	@param    color		16-bit 5-6-5 Color to fill with
*************************************************************************************************/
void DIS_ST7735_fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
	SPI_startWrite();
	
	DIS_ST7735_drawVLine(x0, y0 - r, 2 * r + 1, color);
	DIS_ST7735_fillCircleHelper(x0, y0, r, 3, 0, color);
	
	SPI_endWrite();
}


/************************************************************************************************
	@brief	  Draw a rounded rectangle with fill color
	
    @param    x			Top left corner x coordinate
    @param    y			Top left corner y coordinate
    @param    w			Width in pixels
    @param    h			Height in pixels
    @param    r			Radius of corner rounding
    @param    color		16-bit 5-6-5 Color to draw/fill with
*************************************************************************************************/
void DIS_ST7735_fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color){
	int16_t max_radius = 0;
	
	// 1/2 minor axis
	if(w < h) max_radius = w / 2;
	else max_radius = h / 2;
	
	if(r > max_radius) r = max_radius;
	
	SPI_startWrite();
	DIS_ST7735_fillRect(x + r, y, w - 2 * r, h, color);
	
	// Draw four corners
	DIS_ST7735_fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
	DIS_ST7735_fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
	
	SPI_endWrite();
}



/************************************************************************************************
	@brief    Draw a line. Bresenham's algorithm - thx wikipedia
	
	@param    x0		Start point x coordinate
	@param    y0		Start point y coordinate
	@param    x1		End point x coordinate
	@param    y1		End point y coordinate
	@param    color		16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color){

	int16_t steep = MATH_abs(y1 - y0) > MATH_abs(x1 - x0);
	
	if(steep){
		_swap_int16_t(x0, y0);
		_swap_int16_t(x1, y1);
	}

	if(x0 > x1){
		_swap_int16_t(x0, x1);
		_swap_int16_t(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = MATH_abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if(y0 < y1){
		ystep = 1;
	}else{
		ystep = -1;
	}

	SPI_startWrite();
	
	for(; x0 <= x1; x0++){
		if(steep){
			DIS_ST7735_writePixel(y0, x0, color);
		}else{
			DIS_ST7735_writePixel(x0, y0, color);
		}
		
		err -= dy;
		
		if(err < 0){
			y0 += ystep;
			err += dx;
		}
	}
	
	SPI_endWrite();
}


/************************************************************************************************
	@brief	Draw a triangle with no fill color
	
	@param	x0		Vertex #0 x coordinate
	@param	y0		Vertex #0 y coordinate
	@param	x1		Vertex #1 x coordinate
	@param	y1		Vertex #1 y coordinate
	@param	x2		Vertex #2 x coordinate
	@param	y2		Vertex #2 y coordinate
	@param	color	16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){
	DIS_ST7735_drawLine(x0, y0, x1, y1, color);
	DIS_ST7735_drawLine(x1, y1, x2, y2, color);
	DIS_ST7735_drawLine(x2, y2, x0, y0, color);
}


/************************************************************************************************
	@brief	Draw a triangle with color-fill
	
	@param	x0		Vertex #0 x coordinate
	@param	y0		Vertex #0 y coordinate
	@param	x1		Vertex #1 x coordinate
	@param	y1		Vertex #1 y coordinate
	@param	x2		Vertex #2 x coordinate
	@param	y2		Vertex #2 y coordinate
	@param	color	16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color){

	int16_t a, b, y, last;

	// Sort coordinates by Y order (y2 >= y1 >= y0)
	if(y0 > y1){
		_swap_int16_t(y0, y1);
		_swap_int16_t(x0, x1);
	}
	
	if(y1 > y2){
		_swap_int16_t(y2, y1);
		_swap_int16_t(x2, x1);
	}
	
	if(y0 > y2){
		_swap_int16_t(y0, y2);
		_swap_int16_t(x0, x2);
	}

	SPI_startWrite();
	
	if(y0 == y2){ // Handle awkward all-on-same-line case as its own thing
		a = b = x0;
		
		if(x1 < a)	a = x1;
		else if(x1 > b)	b = x1;
		
		if(x2 < a)	a = x2;
		else if(x2 > b)	b = x2;
		
		DIS_ST7735_drawHLine(a, y0, b - a + 1, color);
		SPI_endWrite();
		return;
	}

	int16_t dx01 = x1 - x0;
	int16_t dy01 = y1 - y0;
	int16_t dx02 = x2 - x0;
	int16_t dy02 = y2 - y0;
	int16_t dx12 = x2 - x1;
	int16_t dy12 = y2 - y1;
	int32_t sa = 0, sb = 0;

	// For upper part of triangle, find scanline crossings for segments
	// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
	// is included here (and second loop will be skipped, avoiding a /0
	// error there), otherwise scanline y1 is skipped here and handled
	// in the second loop...which also avoids a /0 error here if y0=y1
	// (flat-topped triangle).
	if(y1 == y2)
		last = y1; // Include y1 scanline
	else
		last = y1 - 1; // Skip it

	for(y = y0; y <= last; y++){
		a = x0 + sa / dy01;
		b = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		/* longhand:
		a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if(a > b) _swap_int16_t(a, b);
		DIS_ST7735_drawHLine(a, y, b - a + 1, color);
	}
	

	// For lower part of triangle, find scanline crossings for segments
	// 0-2 and 1-2.  This loop is skipped if y1=y2.
	sa = (int32_t)dx12 * (y - y1);
	sb = (int32_t)dx02 * (y - y0);
	
	for(; y <= y2; y++){
		a = x1 + sa / dy12;
		b = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		/* longhand:
		a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
		b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
		*/
		if(a > b) _swap_int16_t(a, b);
		DIS_ST7735_drawHLine(a, y, b - a + 1, color);
	}
	
	SPI_endWrite();
}



/************************************************************************************************
	@brief	Draw a circle outline
	
	@param	x0		Center-point x coordinate
	@param	y0		Center-point y coordinate
	@param	r		Radius of circle
	@param	color	16-bit 5-6-5 Color to draw with
*************************************************************************************************/
void DIS_ST7735_drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color){
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SPI_startWrite();
	DIS_ST7735_writePixel(x0, y0 + r, color);
	DIS_ST7735_writePixel(x0, y0 - r, color);
	DIS_ST7735_writePixel(x0 + r, y0, color);
	DIS_ST7735_writePixel(x0 - r, y0, color);

	while(x < y){
		if(f >= 0){
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		
		x++;
		ddF_x += 2;
		f += ddF_x;

		DIS_ST7735_writePixel(x0 + x, y0 + y, color);
		DIS_ST7735_writePixel(x0 - x, y0 + y, color);
		DIS_ST7735_writePixel(x0 + x, y0 - y, color);
		DIS_ST7735_writePixel(x0 - x, y0 - y, color);
		DIS_ST7735_writePixel(x0 + y, y0 + x, color);
		DIS_ST7735_writePixel(x0 - y, y0 + x, color);
		DIS_ST7735_writePixel(x0 + y, y0 - x, color);
		DIS_ST7735_writePixel(x0 - y, y0 - x, color);
	}
	
	SPI_endWrite();
}



/**************************************************************************
	@brief	Before writing to the RAM, a window must be defined that
			will be written.
			
	@param	x		Top left corner x coordinate
	@param	y		Top left corner x coordinate
	@param  w		Width of window
	@param  h		Height of window
***************************************************************************/
void DIS_ST7735_setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h){
	x += _xstart;
	y += _ystart;

	uint32_t xa = ((uint32_t)x << 16) | (x + w - 1);
	uint32_t ya = ((uint32_t)y << 16) | (y + h - 1);

	DIS_ST7735_writeCommand(ST7735_CASET);		// Column addr set
	SPI_transmit(xa, 32);						// Send parameters

	DIS_ST7735_writeCommand(ST7735_RASET);		// Row addr set
	SPI_transmit(ya, 32);						// Send parameters

	DIS_ST7735_writeCommand(ST7735_RAMWR);		// write to RAM
}



/**************************************************************************
	@brief	Set the cursor position to the specified coordinates.
			Will be used by the drawString and drawChar methods
	@param	x		x coordinate
	@param	y		y coordinate
***************************************************************************/
void DIS_ST7735_setCursor(uint8_t x, uint8_t y){
	cursor_x = x;
	cursor_y = y;
}

void DIS_ST7735_setCursorX(uint8_t x){
	cursor_x = x;
}

void DIS_ST7735_setCursorY(uint8_t y){
	cursor_y = y;
}



/************************************************************************************************
	@brief	Set rotation setting for display
	@param	x		0 to 3 corresponding to 4 cardinal rotations
*************************************************************************************************/
void DIS_ST7735_setRotation(uint8_t x){
	rotation = (x & 3); // can't be higher than 3
	
	//cursor_x = cursor_y = 0;
	
	SPI_startWrite();
	DIS_ST7735_writeCommand(ST7735_MADCTL);
	
	switch(rotation){
		case ROTATE_DEGREE_0:
			_width = WIDTH;
			_height = HEIGHT;
			_xstart = X_OFFSET;
			_ystart = Y_OFFSET;
			DIS_ST7735_writeData(ST7735_MADCTL_REG);
		break;
		case ROTATE_DEGREE_90:
			_width = HEIGHT;
			_height = WIDTH;
			_xstart = Y_OFFSET;
			_ystart = X_OFFSET;
			DIS_ST7735_writeData(ST7735_MADCTL_MV | ST7735_MADCTL_MX | ST7735_RGB_FILTER);
		break;
		case ROTATE_DEGREE_180:
			_width = WIDTH;
			_height = HEIGHT;
			_xstart = X_OFFSET;
			_ystart = Y_OFFSET;
			DIS_ST7735_writeData(ST7735_RGB_FILTER);
		break;
		case ROTATE_DEGREE_270:
			_width = HEIGHT;
			_height = WIDTH;
			_xstart = Y_OFFSET;
			_ystart = X_OFFSET;
			DIS_ST7735_writeData(ST7735_MADCTL_MV | ST7735_MADCTL_MY | ST7735_RGB_FILTER);
		break;
		default:
		break;
	}
	
	SPI_endWrite();
}



/**************************************************************************
    @brief  Draw a single pixel to the display at requested coordinates.
    @param  x			Horizontal position (0 = left)
    @param  y			Vertical position   (0 = top)
    @param  color		16-bit pixel color in '565' RGB format
***************************************************************************/
void DIS_ST7735_writePixel(int16_t x, int16_t y, uint16_t color){
    DIS_ST7735_setAddrWindow(x, y, 1, 1);
	SPI_transmit(color, 16);
}


/**************************************************************************
    @brief  Draw a vertical line on the display. Performs edge clipping and
            rejection.
    @param  x		Horizontal position of first point
    @param  y		Vertical position of first point
    @param  h		Line height in pixels (positive = below first point,
					negative = above first point)
    @param  color	16-bit line color in '565' RGB format
***************************************************************************/
void DIS_ST7735_drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color){

	if((x >= 0) && (x < _width) && h){ // X on screen, nonzero height
	
		if(h < 0){						// If negative height...
			y += h + 1;					// Move Y to top edge
			h = -h;						// Use positive height
		}
		
		if(y < _height){				// Not off bottom
			int16_t y2 = y + h - 1;
				
			if(y2 >= 0){				// Not off top
				// Line partly or fully overlaps screen
				// Clip top
				if(y < 0){
					y = 0;
					h = y2 + 1;
				}
				
				// Clip bottom
				if(y2 >= _height){
					h = _height - y;
				}
				
				DIS_ST7735_fillRect(x, y, lineThickness, h, color);
			}
		}
	}
}


/**************************************************************************
    @brief  Draw a horizontal line on the display. Performs edge clipping and
            rejection.
			
    @param  x		Horizontal position of first point
    @param  y		Vertical position of first point
    @param  w		Line width in pixels (positive = right of first point,
					negative = point of first corner)
    @param  color	16-bit line color in '565' RGB format
***************************************************************************/
void DIS_ST7735_drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color){

	if((y >= 0) && (y < _height) && w){	// Y on screen, nonzero width
		if(w < 0){							// If negative width...
			x += w + 1;						// Move X to left edge
			w = -w;							// Use positive width
		}
		
		if(x < _width){ // Not off right
			int16_t x2 = x + w - 1;
			
			if(x2 >= 0){ // Not off left
				// Line partly or fully overlaps screen
				// Clip left
				if(x < 0){
					x = 0;
					w = x2 + 1;
				}
				
				// Clip right
				if(x2 >= _width){
					w = _width - x;
				}
				
				DIS_ST7735_fillRect(x, y, w, lineThickness, color);
			}
		}
	}
}



/***************************************************************************************
    @brief  Increment the cursor to next line (y axis).
			
	@param	x	left margin
****************************************************************************************/
void DIS_ST7735_gotoNextLine(uint8_t x){
	cursor_y += DIS_ST7735_getLineHeight();
	cursor_x = x;
}


/************************************************************************************************
	@brief	Convert 24-bit RGB to 16-bit 565 RGB
			Adapted from: https://gist.github.com/wirepair/b831cf168882c7013b68c1703bda5250
	
	@param	rgb24		24-bit RGB

	@return	16-bit 565 RGB
*************************************************************************************************/
uint16_t rgbConvert24to16(uint32_t rgb24){
	uint16_t r = (rgb24 & 0xFF0000) >> 16;
	uint16_t g = (rgb24 & 0xFF00) >> 8;
	uint16_t b = rgb24 & 0xFF;
	uint16_t RGB565 = 0;
	
	r = (r * 249 + 1014) >> 11;
	g = (g * 253 + 505) >> 10;
	b = (b * 249 + 1014) >> 11;
	
	RGB565 |= (r << 11);
	RGB565 |= (g << 5);
	RGB565 |= b;
	
	return RGB565;
}


/***************************************************************************************
    @brief  Returns the display width as modified by current rotation
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getDisplayWidth(void){
	return _width;
}


/***************************************************************************************
    @brief  Returns the display height as modified by current rotation
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getDisplayHeight(void){
	return _height;
}


/***************************************************************************************
    @brief  Returns the number of rows (y axis). Use this for (x, y) params.
			For (width, height) params use getDisplayHeight().
			Useful for aligning stuff from bottom side of the display
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getRowCount(void){
	return _height - 1;
}


/***************************************************************************************
    @brief  Returns the number of columns (x axis). Use this for (x, y) params.
			For (width, height) params use getDisplayWidth().
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getColumnCount(void){
	return _width - 1;
}


/***************************************************************************************
    @brief  Returns the x cursor position
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getCursorX(void){
	return cursor_x;
}



/***************************************************************************************
    @brief  Returns the y cursor position
	
    @param  none
****************************************************************************************/
uint8_t DIS_ST7735_getCursorY(void){
	return cursor_y;
}


/***************************************************************************************
    @brief  Returns the height of the text in pixels starting from the baseline.
			E.g: baseline of _g_ is here _
****************************************************************************************/
uint8_t DIS_ST7735_getLineHeight(void){
	if(gfxFont == 0) return 0;
	
	return pgm_read_byte(&gfxFont->yAdvance) * textsize_y;
}


/***************************************************************************************
    @brief  Returns the y coordinate of a specified text line. This can be used with
			fiilRect() to set the text background.
			
    @param  lineNr	Line number starting from 0.
****************************************************************************************/
uint8_t DIS_ST7735_getLineYStart(uint8_t lineNr){
	if(gfxFont == 0) return 0;

	uint8_t yAdvance = pgm_read_byte(&gfxFont->yAdvance);
	uint8_t first = pgm_read_byte(&gfxFont->first);
	uint8_t last = pgm_read_byte(&gfxFont->last);
	uint8_t i = 0;
	int8_t max = 0;
	int8_t yOffset = 0;
	
	GFXglyph *glyph = &(((GFXglyph *)pgm_read_word(&gfxFont->glyph))[0]);
	
	for(i = 0; i < last - first; i++){
		glyph++;
		yOffset = pgm_read_byte(&glyph->yOffset);
		if(yOffset < max) max = yOffset;
	}
	
	max = -max;
	yAdvance *= lineNr;
	
	return yAdvance * textsize_y;
	//return (yAdvance + -yOffset) * textsize_y;
}



/***************************************************************************************
    @brief  Returns the with of a string of characters in pixels

    @param  c		The string
****************************************************************************************/
uint8_t DIS_ST7735_getTextWidth(const char *c){
	if(gfxFont == 0) return 0;
	uint8_t first = pgm_read_byte(&gfxFont->first);
	uint8_t text_width = 1;
	
	while(*c){
		GFXglyph *glyph = &(((GFXglyph *)pgm_read_word(&gfxFont->glyph))[*c - first]);
		text_width += (pgm_read_byte(&glyph->xOffset) + pgm_read_byte(&glyph->xAdvance)) * textsize_x;
		c++;
	}
	
	return text_width;
}



/***************************************************************************************
	@brief	Handles complete sending of commands and data
	
	@param	commandByte       The Command Byte
	@param	dataBytes         A pointer to the Data bytes to send
	@param	numDataBytes      The number of bytes we should send
****************************************************************************************/
static void DIS_ST7735_sendBulkCommand(uint8_t commandByte, const uint8_t *dataBytes, uint8_t numDataBytes){
	SPI_startWrite();

	SPI_DC_LOW_CMD();				// Command mode
	SPI_transmit(commandByte, 8); 	// Send the command byte
	SPI_DC_HIGH_DATA();				// Data mode
	
	// Send parameters
	for(uint8_t i = 0; i < numDataBytes; i++){
		SPI_transmit(pgm_read_byte(dataBytes++), 8);
	}

	SPI_endWrite();
}


/**************************************************************************
	@brief	Write a single command byte to the display. Chip-select must have
			been previously set.
			
	@param	cmd		The command byte
***************************************************************************/
static void DIS_ST7735_writeCommand(uint8_t cmd){
	SPI_DC_LOW_CMD(); // Command mode
	SPI_transmit(cmd, 8);
	SPI_DC_HIGH_DATA(); // Data mode
}


/**************************************************************************
	@brief	Write a single command byte to the display
			
	@param	cmd		The command byte
***************************************************************************/
static void DIS_ST7735_sendCommand(uint8_t cmd){
	SPI_startWrite();
	SPI_DC_LOW_CMD(); // Command mode
	SPI_transmit(cmd, 8);
	SPI_DC_HIGH_DATA(); // Data mode
	SPI_endWrite();
}


/**************************************************************************
	@brief	Write a single data byte to the display. Chip-select must have
			been previously set.
			
	@param	data	The data byte
***************************************************************************/
static void DIS_ST7735_writeData(uint8_t data){
	SPI_transmit(data, 8);
}



/**************************************************************************
	@brief   Read 8 bits of data from display configuration memory (not RAM)

	@param   commandByte	The command register to read data from
	@param   index			The byte index into the command to read from
	
	@return  Unsigned 8-bit data read from display register
***************************************************************************/
uint8_t DIS_ST7735_readcommand8(uint8_t commandByte, int8_t index){
	uint8_t bit = 0;
	uint8_t result = 0;
	
	SPI_startWrite();
	DIS_ST7735_writeCommand(commandByte);
	
	// Disable SPI so we can put the MOSI pin to high impedance 
	SPCR &= ~(1 << SPE0);
	
	// Set SDA (MOSI) pin to hi-Z
	SPI0_PORT &= ~(1 << SPI0_MOSI_PIN);
	SPI0_DDR &= ~(1 << SPI0_MOSI_PIN);
	
	// When using these commands there is a dummy clock at the beginning 
	// of data transmission and here it's being skipped
	if(commandByte == ST7735_RDDID || commandByte == ST7735_RDDST){
		// SCK high
		SPI0_PORT |= (1 << SPI0_SCK_PIN);
		
		// SCK low
		SPI0_PORT &= ~(1 << SPI0_SCK_PIN);
	}

	
	// Receive data from display driver
	do{
		for(bit = 0; bit < 8; bit++){
			
			// SCK high
			SPI0_PORT |= (1 << SPI0_SCK_PIN);
			
			result <<= 1;
			
			// Read the state of SDA pin
			if(SPI0_PINS & (1 << SPI0_MOSI_PIN)) result++;
			
			// SCK low
			SPI0_PORT &= ~(1 << SPI0_SCK_PIN);
		}
		
	}while(index--); // discard bytes up to index
	
	SPI_endWrite();
	
	// Set MOSI as output
	SPI0_DDR |= (1 << SPI0_MOSI_PIN);
	
	// Enable the SPI
	SPCR |= (1 << SPE0);
	
	return result;
}



/**************************************************************************
    @brief	Invert the colors of the display (if supported by hardware)

    @param	i	true = inverted display, false = normal display
***************************************************************************/
void DIS_ST7735_invertDisplay(bool i){
	DIS_ST7735_sendCommand(i ? ST7735_INVON : ST7735_INVOFF);
}



/**************************************************************************
	@brief	Change whether sleep mode is on or off
	
	@param	enable	True if you want sleep mode ON, false OFF
***************************************************************************/
void DIS_ST7735_enableSleep(bool enable){
	DIS_ST7735_sendCommand(enable ? ST7735_SLPIN : ST7735_SLPOUT);
	
	#ifdef TFT_BL_DDR
		if(enable){
			DIS_ST7735_backlightOff();
		}else{
			DIS_ST7735_backlightOn();
		}
	#endif
	
	_delay_ms(120);
}




/**************************************************************************
	@brief	Change whether TE pin (Tearing Effect Line) output is on or off
	
	@param	enable	True if you want the TE pin ON, false OFF
***************************************************************************/
void DIS_ST7735_enableTearing(bool enable){
	DIS_ST7735_sendCommand(enable ? ST7735_TEON : ST7735_TEOFF);
}



/**************************************************************************
	@brief	Change whether idle mode is on or off
	
	@param	enable	True for idle mode on, false for idle mode off
***************************************************************************/
void DIS_ST7735_idleMode(bool enable){
	DIS_ST7735_sendCommand(enable ? ST7735_IDMON : ST7735_IDMOFF);
}


/**************************************************************************
	@brief	Change whether display is on or off
	@param	enable	True if you want the display ON, false OFF
***************************************************************************/
void DIS_ST7735_enableDisplay(bool enable){
	DIS_ST7735_sendCommand(enable ? ST7735_DISPON : ST7735_DISPOFF);
	
	#ifdef TFT_BL_DDR
		if(enable){
			DIS_ST7735_backlightOn();
		}else{
			DIS_ST7735_backlightOff();
		}
	#endif
	
	_delay_ms(120);
}



/**************************************************************************
	@brief  Call before issuing command(s) or data to display. 
			Performs chip-select.
			
	@param  none
***************************************************************************/
static void SPI_startWrite(void){
	SPI_TFT_CS_LOW();
}



/**************************************************************************
	@brief  Call after issuing command(s) or data to display. 
			Performs chip-deselect.
			
	@param  none
***************************************************************************/
static void SPI_endWrite(void){
	SPI_TFT_CS_HIGH();
}


/**************************************************************************
	@brief  SPI setup as Master with the fastest clock fck / 2
	
	@param  none
***************************************************************************/
static void SPI_init(void){
	// Set MOSI and SCK as output
	SPI0_DDR |= (1 << SPI0_MOSI_PIN) | (1 << SPI0_SCK_PIN);
	
	// Enable SPI, Master, set clock rate fck/2
	SPCR = (1 << SPE0) | (1 << MSTR0);
	SPSR = (1 << 0); // set clock rate fck/2
}


/**************************************************************************
	@brief  Transmit up to 4 bytes of data over SPI. The device must be first 
			selected using the appropriate CS pin.
			
	@param  data  		Data in one of the formats: uint8_t, uint16_t, uint32_t
	@param  data_size  	Number of bits: 8, 16 or 32
***************************************************************************/
static void SPI_transmit(uint32_t data, uint8_t data_size){	
	switch(data_size){
		case 8:
			SPDR = (uint8_t)data;
			while(!(SPSR & (1 << SPIF0)));
		break;
		
		case 16:
			SPDR = (uint8_t)(data >> 8);
			while(!(SPSR & (1 << SPIF0)));
			SPDR = (uint8_t)(data);
			while(!(SPSR & (1 << SPIF0)));
		break;
		
		case 32:
			SPDR = (uint8_t)(data >> 24);
			while(!(SPSR & (1 << SPIF0)));
			SPDR = (uint8_t)(data >> 16);
			while(!(SPSR & (1 << SPIF0)));
			SPDR = (uint8_t)(data >> 8);
			while(!(SPSR & (1 << SPIF0)));
			SPDR = (uint8_t)(data);
			while(!(SPSR & (1 << SPIF0)));
	}
}

#endif