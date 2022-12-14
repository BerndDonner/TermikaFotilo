
#ifndef __MLX90621__
#define __MLX90621__

// pruefe, ob wire.h angepasst wurde:

class MLX90621
{
  private:
    uint8_t eeprom_dump_address = 0x50 << 1;
    uint8_t chip_address = 0x60 << 1;
    uint16_t configreg;
    int16_t vcp;
    int16_t acp;
    int16_t acommon;
    int16_t ksta;
    int8_t bcpee;
    float alphacp;
    int8_t tgc;
    uint8_t aiscale;
    uint8_t biscale;
    uint16_t alpha0;
    uint8_t alpha0scale;
    uint8_t deltaalphascale;
    uint16_t epsilon;
    uint8_t irpixels[128];
    float ta;
    int8_t ks4ee;
    uint8_t ksscale;

    uint8_t eepromMLX[256];     // Buffer
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
