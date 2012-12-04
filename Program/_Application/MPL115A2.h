#ifndef __MPL115A2_H
#define __MPL115A2_H 

//stm32
#include "stm32f10x.h"

// pin mappings
#include "qdt_config.h"

#define BAROADRESS 0xC0

// keep measured pressur
extern volatile float mpl115a2_pressure;

// keep measured pressur as array of chars
extern volatile char mpl115a2_pressure_string[10]; 

// read pressure values sensor and save to mpl115a2_pressure ivar
void mpl115a2_read_pressure(void);

void BaroGetPressure(float *PreTemp);
float c2f(uint8_t ch,uint8_t cl,uint8_t nbits,uint8_t fbits,uint8_t zpad);

#endif
