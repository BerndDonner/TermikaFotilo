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
 
#include <SPI.h>
#include <TFT.h>
#include "termika_fotilo.h"
#include "mlx90621.h"
#include "st7735/dis_st7735.h"
#include "st7735/Fonts/font5x7fixed_mono.h"



#define PROFILING
#define RGB(r, g, b)   r, g, b

SPISettings spisettings = SPISettings(4000000L, MSBFIRST, SPI_MODE0);
volatile uint8_t *rsport;
uint8_t           rspinmask;
volatile uint8_t *csport;
uint8_t           cspinmask;



TFT TFTscreen = TFT(CS_, DC_, RST_);    // TFT Konstruktor
MLX90621 MLXtemp;                       // Objekt fuer Tempsensor erzeugen

void setup() {}     // Arduino Schwachsinn

void loop() 
{
  #ifdef PROFILING
    unsigned long time_start, time_stop;
    Serial.begin(38400);
  #endif
  rsport    = portOutputRegister(digitalPinToPort(DC_));
  rspinmask = digitalPinToBitMask(DC_);
  csport    = portOutputRegister(digitalPinToPort(CS_));
  cspinmask = digitalPinToBitMask(CS_);

  initWeight();  
  StartScreen();
  while (!MLXtemp.init())        // MLX90620 init failed
    delay (100);
  
  while (1)   // endlos
  {
    #ifdef PROFILING
      time_start = micros();
    #endif
    OutAmbientTemp();
    OutTempField();
    #ifdef PROFILING
      time_stop = micros();
      Serial.print(F("PROFILING --- Laufzeit der Hauptschleife: "));
      Serial.println(time_stop-time_start); //prints time since program started
    #endif

  }
}

void initWeight()
{
  for (int i = 0; i <= ZOOM; ++i)
  {
    for (int j = 0; j <= ZOOM; ++j)
    {
      Weight[i][j] = (ZOOM-i)*(ZOOM-j) / (float) (ZOOM*ZOOM);
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
  uint8_t i;
  uint8_t R, G, B;
  float gradstep;
  char puffer[10];
  
  TFTscreen.begin ();    // Init Display
  TFTscreen.background (RGB(0, 0, 0));

  TFTscreen.stroke (RGB(0xFF, 0xFF, 0));
  TFTscreen.fill (RGB(20, 100, 50));    
  TFTscreen.rect (0, 0, 159, 20);          // Titelkasten

  TFTscreen.stroke (RGB(0xFF, 0, 0)); 
  TFTscreen.setTextSize (2);
  TFTscreen.text ("Make:", 100, 3);     // 5x7 Font x2

  TFTscreen.stroke (RGB(0xFF , 0xFF, 0xFF));
  TFTscreen.setTextSize (1);
  TFTscreen.text ("TERMIKA FOTILO", 3, 7);  //TODO: Store Strings in Flash: implement TFT.text for flash-mem saves 49bytes

  TFTscreen.stroke (RGB(0xFF, 0x50, 0x50));
  TFTscreen.setTextSize (1);
  TFTscreen.text ("Init...", 0, 112);
  TFTscreen.text ("Please wait", 0, 120);


  TFTscreen.stroke (RGB(127, 127, 127));
  TFTscreen.fill (RGB(0, 0, 100));       
  TFTscreen.rect (0, 27, 16+2, 4+2);                    // Original Daten
  TFTscreen.rect (0, 40, 16*ZOOM+2, 4*ZOOM+2);          // Daten xZOOM
  TFTscreen.rect (0, 75, 15*ZOOM+2, 3*ZOOM+2);      // Daten xZOOM interpoliert

  TFTscreen.setAddrWindow(150, 0+25, 159, 102+25);
  SPI.beginTransaction(spisettings);
  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  for (i=0; i < 103; i++)                 // 103 Farb-Schritte werden angezeigt
  {
    for (uint8_t j = 0; j < 10; ++j)
    {
      uint8_t h_i = ((uint16_t)(i * 13)) >> 3; // 0x555 / 103dec = 13
      uint16_t color = pgm_read_word(colormap + h_i); //colormap[h_i];
      SPI.transfer(color >> 8);
      SPI.transfer(color);
    }
  }

  *csport |= cspinmask;
  SPI.endTransaction();
  TFTscreen.stroke (RGB(0xFF , 0xFF, 0xFF)); 
  TFTscreen.setTextSize (1);
  
  itoa (MINTEMP, puffer, 10);
  TFTscreen.text (puffer, 130, 120);
  
  itoa (MAXTEMP, puffer, 10);
  TFTscreen.text (puffer, 130, 25);        // => 150+20 = 170 Grad-Schritte umfasst der Farbbalken => bei 102 Farb-Schritte = 170/103 = 1,65 °C/Farb-Schritt

  // Zero degree line
  gradstep = (MAXTEMP + abs(MINTEMP)) / 103.0;      // Absoultwert von Min
  i = 25 + (uint8_t)(MAXTEMP / gradstep);
  TFTscreen.line (145, i, 150, i);

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

  TFTscreen.stroke (RGB(0, 0, 0));
  TFTscreen.fill (RGB(0, 0, 0));
  TFTscreen.rect (0, 112, 70, 128-112);          // Alte Zahl loeschen

  TFTscreen.stroke (RGB(0xFF, 0xFF, 0)); 
  TFTscreen.setTextSize (2);

  dtostrf (t, 6, 1, puffer);
  TFTscreen.text (puffer, 0, 112);     // 5x7 Font x2
  TFTscreen.setTextSize (1);
  TFTscreen.text ("\xF7", 74, 112);     // "°C" ASCII 0xF7 fuer Gradzeichen
  TFTscreen.text ("C", 80, 112);    
  
}


/**
  @brief  Grafische Ausgabe
  @param  none
  @return none
*/
void OutTempField(void)
{
  int8_t x, y, xmod, xorg1, xorg2, ymod, yorg1, yorg2; //, xz, yz;
  char puffer[10]; //DEBUG only
  float temps[16][4];
  float i, i1, i2;
  float hue;
  float interpoltemp;
  
  MLXtemp.read_all_irfield (temps);

  // Reihe umkehren, da Sensor nach vorne und Display nach hinten => Temperatur-Feld so wie, auf Display zu zeigen
  for (y = 0; y < 4; y++)
  {
    for (x = 0; x < 8; x++)       
    {
      i = temps[15 - x][y];                       // Wert von rechts retten
      temps[15 - x][y] = temps[x][y];
      temps[x][y] = i;
    }
  }


  // Ausgabe des Temperaturfeldes auf Display
  TFTscreen.setAddrWindow(1, 28, 16, 31);
  SPI.beginTransaction(spisettings);
     
  *rsport |=  rspinmask;
  *csport &= ~cspinmask;

  for (uint8_t yi = 0; yi < 4; ++yi)
  {
    for (uint8_t xi = 0; xi < 16; ++xi)       
    {
      hue = (MAXTEMP - temps[xi][yi]) / (float)(MAXTEMP - MINTEMP);
      uint8_t h_i = ((uint16_t)(hue * 0x555)) >> 3;
      uint16_t color = pgm_read_word(colormap + h_i); //colormap[h_i];
      SPI.transfer(color >> 8);
      SPI.transfer(color);
    }
  }
  *csport |= cspinmask;
  SPI.endTransaction();


  // Ausgabe des Temperaturfeldes auf Display
  for (uint8_t xi = 0; xi < 16; ++xi)       
  {
    TFTscreen.setAddrWindow(xi*ZOOM + 1, 0*ZOOM + 41, (xi+1)*ZOOM, 4*ZOOM + 40);
    SPI.beginTransaction(spisettings);
      
    *rsport |=  rspinmask;
    *csport &= ~cspinmask;

    for (uint8_t yi = 0; yi < 4; ++yi)
    {
      hue = (MAXTEMP - temps[xi][yi]) / (float)(MAXTEMP - MINTEMP);
      uint8_t h_i = ((uint16_t)(hue * 0x555)) >> 3;
      uint16_t color = pgm_read_word(colormap + h_i); //colormap[h_i];

      for(uint8_t yd = 0; yd <  ZOOM; ++yd)
      {
        for(uint8_t xd = 0; xd < ZOOM; ++xd)
        {          
          SPI.transfer(color >> 8);
          SPI.transfer(color);
        }
      }
//      dtostrf (temps[xi][yi], 6, 1, puffer);    // Ausgabe Zahlenwerte
//      Serial.print (puffer);
    }
//    Serial.println();
    *csport |= cspinmask;
    SPI.endTransaction();
  }
//    Serial.println("***");



  // Bilineare Interpolation mit Tabelle
  for(uint8_t xi = 0; xi < 16-1; ++xi)
  {
    TFTscreen.setAddrWindow(xi*ZOOM + 1, 0*ZOOM + 75 + 1, (xi+1)*ZOOM, 3*ZOOM + 75);
    SPI.beginTransaction(spisettings);
      
    *rsport |=  rspinmask;
    *csport &= ~cspinmask;

    for(uint8_t yi = 0; yi <  4-1; ++yi)
    {
      for(uint8_t yd = 0; yd <  ZOOM; ++yd)
      {
        for(uint8_t xd = 0; xd < ZOOM; ++xd)
        {          
          interpoltemp = temps[xi  ][yi  ] * Weight[xd     ][yd     ] +
                         temps[xi+1][yi  ] * Weight[ZOOM-xd][yd     ] +
                         temps[xi  ][yi+1] * Weight[xd     ][ZOOM-yd] +
                         temps[xi+1][yi+1] * Weight[ZOOM-xd][ZOOM-yd];

          hue = (MAXTEMP - interpoltemp) / (float)(MAXTEMP - MINTEMP);
          uint8_t h_i = ((uint16_t)(hue * 0x555)) >> 3;
          uint16_t color = pgm_read_word(colormap + h_i); //colormap[h_i];
          SPI.transfer(color >> 8);
          SPI.transfer(color);
        }
      }
    }
    *csport |= cspinmask;
    SPI.endTransaction();
  }
}
