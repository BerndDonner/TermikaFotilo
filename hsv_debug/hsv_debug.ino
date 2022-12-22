void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  uint8_t r, g, b;

  for (float h = 0.0; h < 1.0; h += 0.001)
  {
    HSVtoRGB(r, g, b, h); 
  }

}

void HSVtoRGB(uint8_t &r, uint8_t &g, uint8_t &b, float h) 
{
   uint8_t i;
   uint8_t f;
   uint16_t h_i;
   uint8_t q, t;

   h_i = h * 0x555; //(16*256 / (float)3);           // sector 0 to 5
   Serial.print("h_i: ");
   Serial.println(h_i);

   i = h_i >> 8;
   f = h_i & 0x00ff;         // factorial part of h

   q = (0x00ff * (0x0100 - (uint16_t) f )) >> 9; //q = (0x00ff * (0x0100 - f )) >> 9;
   t = (0x00ff * f) >> 9;

   Serial.print("q: ");
   Serial.println(q);
   Serial.print("f: ");
   Serial.println(f);

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
