

// Displayanschluesse:
uint8_t CS_  = 10;
uint8_t DC_  = 9;
uint8_t RST_ = 8;  

// Farb-Spektrum
const int8_t MINTEMP = -5;
const uint8_t MAXTEMP = 30;
const uint16_t HUEMAX = 320;    // Schrittweite für Farbstrahl. Eigentlich 360 aber wir wollen die lila Farben etwas vermeiden, weil ähnlich wie rot

const uint8_t ZOOM = 8;         // Vergroesserungfaktor
float Weight[ZOOM+1][ZOOM+1];


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



