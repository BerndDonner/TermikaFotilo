/*
 * Termika Fotilo
 * Make: IV/2017
 * 
 * Arduino Nano mit LCD 128x160 (ST7735) und MLX90621 als Waermebildkamera
 * 
 * ACHTUNG!
 * In den Libraries im User-Verzeichnis
 * \AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.5\libraries\Wire\src\wire.h und
 * \AppData\Local\Arduino15\packages\arduino\hardware\avr\1.8.5\libraries\Wire\src\utility\twi.h
 * muessen unbedingt die #defines fuer
 * BUFFER_LENGTH und
 * TWI_BUFFER_LENGTH
 * auf den Wert 64 geandert werden
 * 
 * Zwei Warning-Ausgaben beim Kompilieren sind normal, weil die SD-Karte auf dem Display nicht genutzt wird
 * 
 */

#include "termika_fotilo.h"
#include "mlx90621.h"            
#include "st7735/dis_st7735.h"  //TODO: This lib is not of the greatest quality. Especally the font handling is a bit crappy. 
#include "st7735/fonts/classicGFXfont.h"




#define PROFILING

MLX90621 MLXtemp;  // Objekt fuer Tempsensor erzeugen

void setup() {}  // Arduino Schwachsinn

void loop()
{
#ifdef PROFILING
  unsigned long time_start, time_stop;
  Serial.begin(38400);
#endif

  initWeight();
  StartScreen();
  while (!MLXtemp.init())  // MLX90620 init failed
    delay(100);

  while (1)  // endlos
  {
#ifdef PROFILING
    time_start = micros();
#endif
    OutAmbientTemp();
    OutTempField();
#ifdef PROFILING
    time_stop = micros();
    Serial.print(F("PROFILING --- Laufzeit der Hauptschleife: "));
    Serial.println(time_stop - time_start);  //prints time since program started
#endif
  }
}

void initWeight()
{
  for (uint8_t i = 0; i <= ZOOM; ++i)
  {
    for (uint8_t j = 0; j <= ZOOM; ++j)
    {
      Weight[i][j] = ( (uint16_t) ((ZOOM - i) * (ZOOM - j)) << 7) / (ZOOM * ZOOM);
    }
  }
}


/**
  @brief  Zeichnet den Grundaufbau auf das Display
  @param  none
  @return none
*/
void StartScreen(void)
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

  DIS_ST7735_fillRect(0, 27, 16 + 2, 4 + 2, 0x000c);
  DIS_ST7735_drawRect(0, 27, 16 + 2, 4 + 2, 0x7bef);
  DIS_ST7735_fillRect(0, 40, 16 * ZOOM + 2, 4 * ZOOM + 2, 0x000c);
  DIS_ST7735_drawRect(0, 40, 16 * ZOOM + 2, 4 * ZOOM + 2, 0x7bef);
  DIS_ST7735_fillRect(0, 75, 15 * ZOOM + 2, 3 * ZOOM + 2, 0x000c);
  DIS_ST7735_drawRect(0, 75, 15 * ZOOM + 2, 3 * ZOOM + 2, 0x7bef);

  SPI_startWrite();
  DIS_ST7735_setAddrWindow(150, 25, 10, 103);
  for (uint8_t i = 0; i < 103; i++)  // 103 Farb-Schritte werden angezeigt
  {
    for (uint8_t j = 0; j < 10; ++j)
    {
      uint8_t h_i = ((uint16_t)(i * 13)) >> 3;         // 0x555 / 103dec = 13
      uint16_t color = pgm_read_word(colormap + h_i);  //colormap[h_i];
      uint8_t color_high = color >> 8, color_low = color;

      SPDR = color_high;
      while (!(SPSR & (1 << SPIF0)));
      SPDR = color_low;
      while (!(SPSR & (1 << SPIF0)));
    }
  }

  SPI_endWrite();

  DIS_ST7735_setFontColor(ST77XX_WHITE);
  DIS_ST7735_setFontSize(1, 1);

  itoa(MINTEMP >> 7, puffer, 10);
  DIS_ST7735_setCursor(130, 120);
  DIS_ST7735_drawString(puffer);

  itoa(MAXTEMP >> 7, puffer, 10);
  DIS_ST7735_setCursor(130, 25);
  DIS_ST7735_drawString(puffer);

  // Zero degree line
  float gradstep = (MAXTEMP + abs(MINTEMP)) / 103.0;  // Absoultwert von Min
  uint8_t i = 25 + (uint8_t)(MAXTEMP / gradstep);
  DIS_ST7735_drawHLine(145, i, 5, ST77XX_WHITE);
}

/**
  @brief  Schreibt die Umgebungstemperatur des Chips aufs Display
  @param  none
  @return none
*/
void OutAmbientTemp(void)
{
  float t = MLXtemp.get_ptat();
  char puffer[10];

  // Serial.println (t);

  DIS_ST7735_fillRect(0, 112, 71, 17, ST77XX_BLACK);  // Alte Zahl loeschen  ST77XX_YELLOW

  DIS_ST7735_setFontColor(ST77XX_YELLOW);
  DIS_ST7735_setFontSize(2, 2);
  DIS_ST7735_setCursor(0, 112);

  dtostrf(t, 6, 1, puffer);
  DIS_ST7735_drawString(puffer);

  DIS_ST7735_setFontSize(1, 1);
  DIS_ST7735_setCursor(74, 112);
  DIS_ST7735_drawChar(74, 112, '\xF8');  // "°C" ASCII 0xF8 fuer Gradzeichen
  DIS_ST7735_setCursor(80, 112);
  DIS_ST7735_drawChar(80, 112, 'C');
}


/**
  @brief  Grafische Ausgabe
  @param  none
  @return none
*/
void OutTempField(void)
{
  int8_t x, y, xmod, xorg1, xorg2, ymod, yorg1, yorg2;  //, xz, yz;
  char puffer[10];                                      //DEBUG only
  int16_t temps[16][4];
  int16_t interpoltemp;

  MLXtemp.read_all_irfield(temps);
  
  // Ausgabe des Temperaturfeldes auf Display
  SPI_startWrite();
  DIS_ST7735_setAddrWindow(1, 28, 16, 4);

  for (uint8_t yi = 0; yi < 4; ++yi)
  {
    for (uint8_t xi = 0; xi < 16; ++xi)
    {
      uint8_t h_i = (((int32_t)(MAXTEMP - temps[xi][yi])) * 0x555) / (MAXTEMP - MINTEMP) >> 3;
      uint16_t color = pgm_read_word(colormap + h_i);  //colormap[h_i];
      uint8_t color_high = color >> 8, color_low = color;

      SPDR = color_high;   //TODO: Double buffering via USART in SPI mode???
      while (!(SPSR & (1 << SPIF0)));
      SPDR = color_low;
      while (!(SPSR & (1 << SPIF0)));
//      sprintf (puffer, "%d   ", color);    // Ausgabe Zahlenwerte
//      Serial.print (puffer);
 
    }
//    Serial.println();
 }
// Serial.println("Ende Farbe ***");


  // Ausgabe des Temperaturfeldes auf Display
  for (uint8_t xi = 0; xi < 16; ++xi)
  {
    DIS_ST7735_setAddrWindow(xi * ZOOM + 1, 0 * ZOOM + 41, ZOOM, 4 * ZOOM);

    for (uint8_t yi = 0; yi < 4; ++yi)
    {
      uint8_t h_i = (((int32_t)(MAXTEMP - temps[xi][yi])) * 0x555) / (MAXTEMP - MINTEMP) >> 3;
      uint16_t color = pgm_read_word(colormap + h_i);  //colormap[h_i];

      for (uint8_t yd = 0; yd < ZOOM; ++yd)
      {
        for (uint8_t xd = 0; xd < ZOOM; ++xd)
        {
          uint8_t color_high = color >> 8, color_low = color;

          SPDR = color_high;
          while (!(SPSR & (1 << SPIF0)));
          SPDR = color_low;
          while (!(SPSR & (1 << SPIF0)));
        }
      }
//      sprintf (puffer, "%d   ", temps[xi][yi]);    // Ausgabe Zahlenwerte
//      Serial.print (puffer);
    }
//    Serial.println();
  }
//  Serial.println("Ende Temperaturen ***");


  // Bilineare Interpolation mit Tabelle
  for (uint8_t xi = 0; xi < 16 - 1; ++xi)
  {
    DIS_ST7735_setAddrWindow(xi * ZOOM + 1, 0 * ZOOM + 75 + 1, ZOOM, 3 * ZOOM);
    for (uint8_t yi = 0; yi < 4 - 1; ++yi)
    {
      for (uint8_t yd = 0; yd < ZOOM; ++yd)
      {
        for (uint8_t xd = 0; xd < ZOOM; ++xd)
        {
          interpoltemp = ((int32_t) temps[xi][yi] * Weight[xd][yd] + (int32_t) temps[xi + 1][yi] * Weight[ZOOM - xd][yd] + (int32_t) temps[xi][yi + 1] * Weight[xd][ZOOM - yd] + (int32_t) temps[xi + 1][yi + 1] * Weight[ZOOM - xd][ZOOM - yd]) >> 7;

          uint8_t h_i = (((int32_t)(MAXTEMP - interpoltemp)) * 0x555) / (MAXTEMP - MINTEMP) >> 3;
          uint16_t color = pgm_read_word(colormap + h_i);  //colormap[h_i];
          uint8_t color_high = color >> 8, color_low = color;

          SPDR = color_high;
          while (!(SPSR & (1 << SPIF0)));
          SPDR = color_low;
          while (!(SPSR & (1 << SPIF0)));
        }
      }
    }
  }
  SPI_endWrite();
}
