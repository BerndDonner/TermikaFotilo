/*
 * Library für MLX90621 IT-Sensor
 * 
 * Andere Typen sind aehnlich aber nutzen andere Berechnung
 * 
 * BD: TODO: Inconsistent ERROR Codes!!!
 * BD: get rid of delays? 
 */


#include <Arduino.h>
#include <avr/io.h>
#include "i2cmaster.h"

#include "mlx90621.h"

/**
  @brief  Konstruktor
  @param  none
  @return none
*/
MLX90621::MLX90621(void)
{
  i2c_init(); 
}

/**
  @brief  Initialisiert den Sensor. Einmal zu Beginn
  @param  none
  @return 0 = Fehler, 1 = OK
*/
uint8_t MLX90621::init (void)
{
	delay(5);   // erst ab 5ms nach power on reset POR
 
  if (!read_eeprom()) 
    return 0;
  write_trim (mem.eepromMLX[0xF7]);                       // Trimwert im EEPROM-Inhalt Adresse 0xF7
  write_config (mem.eepromMLX[0xF5], mem.eepromMLX[0xF6]);    // Normal mode (no sleep), I2C FM+ mode enabled, ADC low reference enabled 

  configreg = read_config();

  float ta = get_ptat();      
  if (ta > 300 || ta < -20)      // out of bounds, wichtig zu pruefen
    return 0;

  int16_t  vcp = read_compensation();                                  // Compensation pixel 
  int16_t  acp = (int16_t)( mem.eepromMLX[0xD4] << 8 ) | mem.eepromMLX[0xD3];           // Compensation pixel individual offset coefficient
  int8_t   bcpee = (int8_t)mem.eepromMLX[0xD5];                                      // Individual Ta dependence (slope) of the compensation pixel offset
  int8_t   tgc = (int8_t)mem.eepromMLX[0xD8];                                      // Thermal gradient coefficient
  uint8_t  aiscale = mem.eepromMLX[0xD9] >> 4;                             // [7:4] – Scaling coeff for the IR pixels offset
  uint8_t  biscale = mem.eepromMLX[0xD9] & 0x0F;                           // [3:0] – Scaling coeff of the IR pixels offset Ta dependence
  uint16_t alpha0 = ( mem.eepromMLX[0xE1] << 8 ) | mem.eepromMLX[0xE0];        // Common sensitivity coefficient of IR pixels
  uint8_t  alpha0scale = mem.eepromMLX[0xE2];                              // Scaling coefficient for common sensitivity
  uint8_t  deltaalphascale = mem.eepromMLX[0xE3];                          // Scaling coefficient for individual sensitivity
  float alphacp = (( mem.eepromMLX[0xD7] << 8 ) | mem.eepromMLX[0xD6]) / (float) (pow (2, alpha0scale) * pow (2, 3 - ( (configreg >> 4) & 0x03)));       // Sensitivity coefficient of the compensation pixel
  epsilon = ( mem.eepromMLX[0xE5] << 8 ) | mem.eepromMLX[0xE4] ;       // Emissivity
  int16_t acommon = (int16_t)(( mem.eepromMLX[0xD1] << 8 ) | mem.eepromMLX[0xD0]);    // IR pixel common offset coefficient
  int16_t ksta = (int16_t)(( mem.eepromMLX[0xD7] << 8 ) | mem.eepromMLX[0xD6]);   // KsTa (fixed scale coefficient = 20)
  int8_t ks4ee = (int8_t)mem.eepromMLX[0xC4];
  uint8_t ksscale = mem.eepromMLX[0xC0];

  float bcp = bcpee / (pow (2, biscale) * pow (2, 3 - ( (configreg >> 4) & 0x03)));           //keine Arraygröße nur einmal berechnen!!!
  float vircpoffsetcompensated = vcp - (acp + bcp * (ta - 25.0) );                                  //einmal für ta berechnen!!

  // 7.3.3.1
  ks4 = ks4ee / pow (2, ksscale + 8); //nur einmal berechnen
  
  // 7.3.3
  tak4 = pow ( (ta + 273.15), 4);

  for (uint8_t i = 0; i < 64; i++)
  {
    uint8_t deltaai = mem.eepromMLX[i]; //deltaai --> ai -->
    float bi = (int8_t)mem.eepromMLX[0x40 + i];  //bi, ai --> bei gegebenen ta offset für vir 
    uint8_t deltaalpha = mem.eepromMLX[0x80 + i]; //----> alpha, ta ---> alphacomp

    float ai = (acommon + deltaai * pow (2, aiscale)) / pow (2, ( (configreg >> 4) & 0x03));      // Bit 5:4 Config Register
    bi = bi / (pow (2, biscale) * pow (2, 3 - ( (configreg >> 4) & 0x03)));
    viroffset[i] = ai + bi * (ta - 25.0) + ( (tgc / 32.0) *  vircpoffsetcompensated); //einmal offset für ta

    // 7.3.3.2
    float alpha = ( ( alpha0 / pow (2, alpha0scale) ) + ( deltaalpha / pow (2, deltaalphascale) ) ) / (float) pow (2, 3 - ( (configreg >> 4) & 0x03)) ;
    alphacomp[i] = (1 + (ksta/pow (2, 20)) * (ta - 25.0)) * (alpha - tgc * alphacp);
  }
  
  return 1;
}

/**
  @brief  Liest das IR-Feld einmal aus und berechnet die Temperaturen fuer jeden einzelnen IR-Sensor => temperatures[]
  @param  none
  @return none
*/
void MLX90621::read_all_irfield (float temperatures[16][4])
{
  uint8_t x, y, i;
  int16_t vir;
  
  while (test_por ())
  {
    delay (10);
//    Serial.print ("Neustart");
    init ();
  }

   if (!read_ir ())
     return;
    
  for (i = 0; i < 64; i++)
  {
    vir = (int16_t)(mem.irpixels[i * 2 + 1] << 8 | mem.irpixels[i * 2]);      

    // Thermal Gradient Compensation (TGC)
    float virtgccompensated = vir - viroffset[i];
    
    // Emissivity compensation
    float vircompensated = virtgccompensated / (epsilon / 32768); //Ganzzahldivision
    
    float sx = ks4 * pow ( ( ( pow (alphacomp[i], 3) * vircompensated ) + ( pow  ( alphacomp[i], 4 ) * tak4 ) ), (1 / 4.0) );              // x. Wurzel aus y = y^(1/x)
    float to = pow ( (vircompensated / ( alphacomp[i] * (1 - ks4 * 273.15) + sx ) ) + tak4, (1 / 4.0) ) - 273.15;

    temperatures[i%16][(uint8_t)i/16] = to;      // yeah, wir haben die reale Temperatur
  }
}

/**
  @brief  Liest 256 Bytes aus dem EEPROM und speichert die Daten in mem.eepromMLX
  @param  none
  @return none
*/
uint8_t MLX90621::read_eeprom (void)
{
  uint16_t i;

  i2c_start_wait(eeprom_dump_address+I2C_WRITE);   // Adresse des Chips: Whole EEPROM dump
  i2c_write(0x00);                                 // Command: Read the whole EEPROM (start address)
  if (i2c_rep_start(eeprom_dump_address+I2C_READ))
    return 0;

  for (i = 0; i < (0xFF - 1); i++)                 // read all 0xFF bytes of the EEPROM
    mem.eepromMLX[i] = i2c_readAck();

  mem.eepromMLX[i] = i2c_readNak();
  i2c_stop();

  return 1;
}

/**
  @brief  Speichert den Oszillator-Trim-Wet aus dem EEPROM (HSB immer 0)
  @param  Trimwert
  @return none
*/
void MLX90621::write_trim (uint8_t trimvalue)
{
  i2c_start_wait(chip_address+I2C_WRITE); // Adresse des Chips zum speichern des Trimwertes
  i2c_write(0x04);                        // Command: write trim value
  i2c_write(trimvalue - 0xAA);            // LSByte check           
  i2c_write(trimvalue);                   // LSB    
  i2c_write((uint8_t) (0x00 - 0xAA));     // HSByte check                  
  i2c_write(0x00);                        // HSB/MSB
  i2c_stop();                            
}

/**
  @brief  Speichert die zwei Konfigurations-Bits aus dem EEPROM
  @param  LSB und HSB Konfibytes
  @return none
*/
void MLX90621::write_config (uint8_t lsb, uint8_t hsb)
{
  i2c_start_wait(chip_address+I2C_WRITE);   // Adresse des Chips zum speichern der Konfigwerte
  i2c_write(0x03);                          // Command: write config values
  i2c_write(lsb - 0x55);                    // LSByte check   
  i2c_write(lsb);                           // LSB
  i2c_write(hsb - 0x55);                    // HSByte check   
  i2c_write(hsb);                           // HSB/MSB
  i2c_stop();                            
}

/**
  @brief  Liest das config-Register ein
  @param  none
  @return Config Register oder -1 wenn Fehler
*/
int32_t MLX90621::read_config()
{
  uint16_t configreg;
  
  i2c_start_wait(chip_address+I2C_WRITE);     // Adresse des Chips
  i2c_write(0x02);                            // command
  i2c_write(0x92);                            // start address
  i2c_write(0x00);                            // Address step
  i2c_write(0x01);                            // Number of reads

  if (i2c_rep_start(chip_address+I2C_READ))   // receive 2 Bytes
    return -1;

  configreg = i2c_readAck();                  // LSB
  configreg |= (i2c_readNak() << 8);          // OR HSB
  i2c_stop();

  return configreg;
}

/**
  @brief  Prueft, ob POR/Brown-out Flag gesetzt
  @param  none
  @return 0 = wahr => init notwendig; 1 = OK
*/
uint8_t MLX90621::test_por (void)
{
  return (read_config () & 0x400);    // POR = 11. Bit 
}


/**
  @brief  Liest die Absolute ambient temperature data of the device itself (package temperature)
  @param  none
  @return PTAT Wert (HSB/LSB) oder -1, wenn Fehler
*/
int32_t MLX90621::read_ptat (void)
{
  int32_t ptat = 0;
  
  i2c_start_wait(chip_address+I2C_WRITE);     // Adresse des Chips
  i2c_write(0x02);                            // command
  i2c_write(0x40);                            // start address
  i2c_write(0x00);                            // Address step
  i2c_write(0x01);                            // Number of reads

  if (i2c_rep_start(chip_address+I2C_READ))   // receive 2 Bytes
    return -1;

  ptat = i2c_readAck();                       // LSB
  ptat |= (i2c_readNak() << 8);               // OR HSB
  i2c_stop();

  return ptat;
}

/**
  @brief  Liefert die Absolute ambient temperature data of the device itself (package temperature)
  @param  none
  @return Temperatur in °C
*/
float MLX90621::get_ptat (void)
{
  uint8_t exp1, exp2;
  float kt1, kt2, vth, tmp;
  int32_t ptat;
  
  
  do {
    ptat = read_ptat();
    if (ptat == -1)       // wenn Fehler, dann neu init
    {
      delay (1000);
      init();
    }
  } while (ptat == -1);
  
  exp2 = ( (configreg >> 4) & 0x03);      // Bit 5:4 Config Register

  vth = ( mem.eepromMLX[0xDB] << 8 ) | mem.eepromMLX[0xDA];
  if (vth > 32767)
    vth -= 65536;
  vth = vth / pow (2, 3-exp2);

  kt1 = (( mem.eepromMLX[0xDD] << 8 ) | mem.eepromMLX[0xDC]);
  if (kt1 > 32767)
    kt1 -= 65536;

  exp1 = ( mem.eepromMLX[0xD2] >> 4);               // Bit 7:4
  kt1 = kt1 / ( pow (2, exp1) * pow (2, 3-exp2) );
  
  kt2 = (( mem.eepromMLX[0xDF] << 8 ) | mem.eepromMLX[0xDE]);
  if (kt2 > 32767)
    kt2 -= 65536;
  exp1 = (mem.eepromMLX[0xD2] & 0x0F);             // Bit 3:0
  kt2 = kt2 / ( pow (2, exp1+10) * pow (2, 3-exp2) );
  
  return (((kt1 * -1.0) + sqrt( kt1*kt1 - (4 * kt2) * (vth - ptat) )) / (2 * kt2) ) + 25.0;
}

/**
  @brief  Liest das compensation pixel
  @param  none
  @return CP Wert (HSB/LSB) oder -1, wenn Fehler
*/
int16_t MLX90621::read_compensation (void)
{
  int8_t cp = 0;
  
  i2c_start_wait(chip_address+I2C_WRITE);     // Adresse des Chips
  i2c_write(0x02);                            // command
  i2c_write(0x41);                            // start address
  i2c_write(0x00);                            // Address step
  i2c_write(0x01);                            // Number of reads

  if (i2c_rep_start(chip_address+I2C_READ))   // receive 2 Bytes
    return -1;

  cp = i2c_readAck();                         // LSB
  cp |= (i2c_readNak() << 8);                 // OR HSB
  i2c_stop();

  return cp;
}

/**
  @brief  Read whole IR frame and save it
  @param  none
  @return 0 wenn Fehler,1 = OK
*/
uint8_t MLX90621::read_ir (void)
{
  uint8_t i, j;
  
  i2c_start_wait(chip_address+I2C_WRITE);     // Adresse des Chips
  i2c_write(0x02);                            // read command, page 31/44
  i2c_write(0x00);                            // start address
  i2c_write(0x01);                            // Address step
  i2c_write(0x40);                            // read 0x40=64 words <---> read 0x40*2 = 128 bytes

  if (i2c_rep_start(chip_address+I2C_READ))   // receive 128 Bytes
    return 0;
  
  for (i=0; i < 0x10-1; i++)                  //loop unrolling is possible here for the compiler
  {                                           //it would only be two loops iff the last read would not need a Not Acknowledge
    for (j = 0; j < 0x04; j++)                //basically transposing the array of words that is read as an array of bytes
    {
      mem.irpixels[(i + 0x10*j)*2]     = i2c_readAck();
      mem.irpixels[(i + 0x10*j)*2 + 1] = i2c_readAck();
    }
  }

  for (j = 0; j < 0x04-1; j++)
  {
    mem.irpixels[(i + 0x10*j)*2]     = i2c_readAck();
    mem.irpixels[(i + 0x10*j)*2 + 1] = i2c_readAck();
  }

  mem.irpixels[(i + 0x10*j)*2]     = i2c_readAck();
  mem.irpixels[(i + 0x10*j)*2 + 1] = i2c_readNak();

  i2c_stop();

  return 1;
}




















