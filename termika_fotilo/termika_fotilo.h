

// Displayanschluesse:
uint8_t CS_  = 10;
uint8_t DC_  = 9;
uint8_t RST_ = 8;  

// Farb-Spektrum
const int8_t MINTEMP = -5;
const uint8_t MAXTEMP = 30;
const uint16_t HUEMAX = 320;    // Schrittweite für Farbstrahl. Eigentlich 360 aber wir wollen die lila Farben etwas vermeiden, weil ähnlich wie rot

const uint8_t ZOOM = 7;         // Vergroesserungfaktor
const float Weight[ZOOM+1][ZOOM+1] = {
  {1.000, 0.857, 0.714, 0.571, 0.429, 0.286, 0.143, 0.000},
  {0.857, 0.735, 0.612, 0.490, 0.367, 0.245, 0.122, 0.000},
  {0.714, 0.612, 0.510, 0.408, 0.306, 0.204, 0.102, 0.000},
  {0.571, 0.490, 0.408, 0.326, 0.245, 0.163, 0.082, 0.000},
  {0.429, 0.367, 0.306, 0.245, 0.184, 0.123, 0.061, 0.000},
  {0.286, 0.245, 0.204, 0.163, 0.123, 0.082, 0.041, 0.000},
  {0.143, 0.122, 0.102, 0.082, 0.061, 0.041, 0.021, 0.000},
  {0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000, 0.000}};

// Allgemein
// u = 1/(Z*Z);
// const float Weight[ZOOM+1][ZOOM+1] = {
//   {(Z-0)*(Z-0), (Z-0)*(Z-1), (Z-0)*(Z-2), (Z-0)*(Z-3),..., (Z-0)*1, (Z-0)*0},
//   {(Z-1)*(Z-0), (Z-1)*(Z-1), (Z-1)*(Z-2), (Z-1)*(Z-3),..., (Z-1)*1, (Z-1)*0},
//   {(Z-2)*(Z-0), (Z-2)*(Z-1), (Z-2)*(Z-2), (Z-2)*(Z-3),..., (Z-2)*1, (Z-2)*0},
//   {...},
//   {...},
//   {...},
//   {1*(Z-0),     1*(Z-1),     1*(Z-2),     1*(Z-3)    ,..., 1*1,     1*0},
//   {0*(Z-0),     0*(Z-1),     0*(Z-2),     0*(Z-3)    ,..., 0*1,     0*0}} multipliziert mit u;


void StartScreen(void);
void OutAmbientTemp(void);
void HSVtoRGB(float &, float &, float &, float, float, float);
void OutTempField(void);
float LinInterpol (float, float, uint8_t, uint8_t, uint8_t);
float FindMinTemp (float *);
float FindMaxTemp (float *);



