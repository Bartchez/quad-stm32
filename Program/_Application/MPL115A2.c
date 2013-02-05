#include "i2c.h"
#include "MPL115A2.h"

//others
#include <stdio.h>
#include <string.h>

// keep measured pressur
volatile float mpl115a2_pressure;
volatile float mpl115a2_temp;
volatile char mpl115a2_pressure_string[20]; 

//*******************************************************************************************
void mpl115a2_read_pressure(void) 
{
	// temp var
	float pomiar[2];

	// get temperature and pressure values
	BaroGetPressure(pomiar);

	// save pressure value
	mpl115a2_pressure = pomiar[0]; 

	// save temp value
	mpl115a2_temp = pomiar[1];

	// create output string	
    sprintf(mpl115a2_pressure_string, "X%.3f,%.3f", mpl115a2_pressure, mpl115a2_temp);    

//	printf("\r\n Pressure: %4.1f hPa ", pomiar[0]);          //wyslanie po uart pomiarow
//	printf("     Temperatura: %2.1f ^C", pomiar[1]);
}

//******************************************************************************************* do obliczania wartosci
float c2f(uint8_t ch,uint8_t cl,uint8_t nbits,uint8_t fbits,uint8_t zpad)
{
	float x;
    x=(short)((ch << 8 | cl) >> 16 - nbits) / (1 << fbits + zpad) ;
    return x;
}

//******************************************************************************************* odczytuje i oblicza cisnienie i temperature
void BaroGetPressure(float *PreTemp)
{
	uint8_t buf[16];
	float padc, tadc, a0, b1, b2, c12, c11, c22, pcomp;
	uint32_t a;

	I2C_WriteOneByte(PRESSURE_I2C, BAROADRESS, 0x12, 0x01);

	//wait ~5 ms
	a=75000;
	while (a>3)    
	{
		a--;
	}

	I2C_Read(PRESSURE_I2C, BAROADRESS, 0x00, buf, 16);

    padc = buf[0] << 2 | buf[1] >> 6;
    tadc = buf[2] << 2 | buf[3] >> 6;
    a0 = c2f(buf[4], buf[5], 16, 3, 0);
    b1 = c2f(buf[6], buf[7], 16, 13, 0);
    b2 = c2f(buf[8], buf[9], 16, 14, 0);
    c12 = c2f(buf[10], buf[11], 14, 13, 9);
    c11 = c2f(buf[12], buf[13], 11, 10, 11);
    c22 = c2f(buf[14], buf[15], 11, 10, 15);
	
	pcomp = a0 + (b1 + c11 * padc + c12 * tadc) * padc + (b2 + c22 * tadc) * tadc;
	PreTemp[0] = pcomp * 650 / 1023 + 500;
    PreTemp[1] = (25 + ((tadc - 498.0) / -5.35));
}

