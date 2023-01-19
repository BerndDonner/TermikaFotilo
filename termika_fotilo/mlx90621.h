
#ifndef __MLX90621__
#define __MLX90621__

class MLX90621
{
  private:
    const uint8_t eeprom_dump_address = 0x50 << 1;
    const uint8_t chip_address = 0x60 << 1;
    uint16_t configreg;
    uint16_t epsilon;
    union {
      uint8_t eepromMLX[256];     // die ersten 3*64 = 192 Bytes werden nur während der Initialisierung benötigt
      uint8_t irpixels[128];
    } mem;
    float viroffset[64];
    float alphacomp[64];
    float ks4;
    float tak4;


    uint8_t read_eeprom (void);
    void write_trim (uint8_t);
    void write_config (uint8_t, uint8_t);
    int32_t read_config (void);
    uint8_t test_por (void);
	  int32_t read_ptat (void);
	  int16_t read_compensation (void);
    uint8_t read_ir (void);
  
	public:
    MLX90621 (void);
		uint8_t init (void);
    void read_all_irfield (float [16][4]);
    float get_ptat (void);
};

#endif
