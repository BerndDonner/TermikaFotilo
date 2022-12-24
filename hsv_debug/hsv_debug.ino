void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  uint8_t r, g, b;
  uint16_t colornew;
  char c[10];

  for (uint16_t h_i = 0; h_i <= 0x0555; h_i += 8)
  {
        
    HSVtoRGBtable(r, g, b, h_i); 
    uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    sprintf(c, "0x%04x, ", color);
    Serial.print(c);
  }

/**
  for (float h = 0.0; h < 1.0; h += 0.0001)
//  float h = 0.1000;
  {
//    HSVtoRGBnew(r, g, b, h);
//    uint16_t colornew = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3); 
    HSVtoRGBold(r, g, b, h); 
    uint16_t colorold = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);

    if (colornew != colorold)
    {
//      Serial.print("New: ");
//      Serial.print(colornew, HEX);
//      Serial.print(" --- Old: ");
      Serial.print(colorold, HEX);
      Serial.print(" huevalue: ");
      Serial.println(h, 4);
      colornew = colorold;
    }
  }
*/
}

void HSVtoRGBnew(uint8_t &r, uint8_t &g, uint8_t &b, float h) 
{
   uint8_t i;
   uint16_t f;
   uint16_t h_i;
   uint8_t q, t;

   h_i = h * 0x555; //(16*256 / (float)3);           // sector 0 to 5
//   Serial.print("h_i: ");
//   Serial.println(h_i);

   HSVtoRGBtable(r, g, b, h_i); 
}


void HSVtoRGBtable(uint8_t &r, uint8_t &g, uint8_t &b, uint16_t h_i) 
{
   uint8_t i;
   uint16_t f;
   uint8_t q, t;

   i = h_i >> 8;
   f = h_i & 0x00ff;         // factorial part of h

   q = (0x00ff * (0x0100 - f )) >> 9; //q = (0x00ff * (0x0100 - f )) >> 9;
   t = (0x00ff * f) >> 9;

/*
   Serial.print("NEW i: ");
   Serial.println(i);
   Serial.print("NEW f: ");
   Serial.println(f);
   Serial.print("NEW q: ");
   Serial.println(q, HEX);
   Serial.print("NEW t: ");
   Serial.println(t, HEX);
*/

   switch( i ) {
      case 0: r = 127; g = t;   b = 0;   break;
      case 1: r = q;   g = 127; b = 0;   break;
      case 2: r = 0;   g = 127; b = t;   break;
      case 3: r = 0;   g = q;   b = 127; break;
      case 4: r = t;   g = 0;   b = 127; break;
      default:  // case 5:
              r = 127; g = 0;   b = q;   break;
   }
}


void HSVtoRGBold(uint8_t &r, uint8_t &g, uint8_t &b, float h) 
{
   int i;
   float f;
   uint8_t q, t;
   h *= (16 / (float)3);           // sector 0 to 5
   i = floor( h );
   f = h - i;         // factorial part of h

   q = 255*0.5 * ( 1 - f );
   t = 255*0.5 * f;

/*
   Serial.print("OLD i: ");
   Serial.println(i);
   Serial.print("OLD f: ");
   Serial.println(f, 4);
   Serial.print("OLD q: ");
   Serial.println(q, HEX);
   Serial.print("OLD t: ");
   Serial.println(t, HEX);
*/

   switch( i ) {
      case 0: r = 127; g = t;   b = 0;   break;
      case 1: r = q;   g = 127; b = 0;   break;
      case 2: r = 0;   g = 127; b = t;   break;
      case 3: r = 0;   g = q;   b = 127; break;
      case 4: r = t;   g = 0;   b = 127; break;
      default:  // case 5:
              r = 127; g = 0;   b = q;   break;
   }
}


void loop()
{

  // put your main code here, to run repeatedly:

}
