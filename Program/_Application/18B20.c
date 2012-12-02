#include "18B20.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
void ds18b20_read_temps(void) {
	uint8_t i = 0;	
									 
	// get array of sensors
	static uint16_t sensors[] = {TEMPERATURE_BIT_1, TEMPERATURE_BIT_2, TEMPERATURE_BIT_3,
						   		 TEMPERATURE_BIT_4, TEMPERATURE_BIT_5, TEMPERATURE_BIT_6};

	// iterate all sensors and get termerature
	for (i=0; i<6; ++i) {
		temp_measurements[i] = ds18b20_read_temp(sensors[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Onewire_OUT_PULL_UP(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStruct);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void Onewire_OUT_FLOATING(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStruct);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void Onewire_IN_FLOATING(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void Onewire_IN_PULL_UP(uint16_t pin)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStruct);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void delay_us(u16 us)
{
	us=15*us;
	while(--us);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t onewire_reset(uint16_t pin) 
{
	uint8_t presence = 1;
	Onewire_OUT_PULL_UP(pin);
	GPIO_ResetBits(TEMPERATURE_PORT, pin);
	delay_us(450);
	GPIO_SetBits(TEMPERATURE_PORT, pin);
	delay_us(60);
	Onewire_IN_PULL_UP(pin); 
	delay_us(10);
	presence = GPIO_ReadInputDataBit(TEMPERATURE_PORT, pin);
	while(!(GPIO_ReadInputDataBit(TEMPERATURE_PORT, pin)));
	Onewire_OUT_PULL_UP(pin);
	GPIO_SetBits(TEMPERATURE_PORT, pin);
	return presence;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t onewire_read(uint16_t pin)
{
    u8 data=0,i=0;
    for(i=0;i<8;i++) {
		Onewire_OUT_PULL_UP(pin);
		GPIO_ResetBits(TEMPERATURE_PORT, pin);
		data = data>>1;
		GPIO_SetBits(TEMPERATURE_PORT, pin);
		Onewire_IN_PULL_UP(pin);
		delay_us(8);
		
		if(GPIO_ReadInputDataBit(TEMPERATURE_PORT, pin)) {
			data|=0x80;
		}
	
		Onewire_OUT_PULL_UP(pin);
		GPIO_SetBits(TEMPERATURE_PORT, pin);
		delay_us(60);
    }

    return(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void onewire_write(uint8_t data, uint16_t pin)
{
	u8 i=0;
	Onewire_OUT_PULL_UP(pin);
	GPIO_SetBits(TEMPERATURE_PORT, pin);
	delay_us(16);
    
	for (i=0;i<8;i++) {
		GPIO_ResetBits(TEMPERATURE_PORT, pin);
		if (data&0x01) {
			GPIO_SetBits(TEMPERATURE_PORT, pin);
		} else {					 
			GPIO_ResetBits(TEMPERATURE_PORT, pin);
    	}

    	data = data>>1;
		delay_us(40);  //65
		GPIO_SetBits(TEMPERATURE_PORT, pin);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
float ds18b20_read_temp(uint16_t pin) {
	uint8_t lowByte, highByte;
	int8_t temperature, fraction;
	int8_t sign = 1;
	int16_t tt;

	// Reset
    if (onewire_reset(pin)) return 255;
	// Skip ROM			
    onewire_write(SKIPROM, pin);	
	// Start Conversion	
    onewire_write(STARTCONVO, pin);		

	// Reset
	if (onewire_reset(pin)) return 255;
				
	// Skip ROM		
	onewire_write(SKIPROM, pin);    	
	// Read Scratch Pad  
	onewire_write(TEMPCOMMAND, pin);		

	// read LS & MS bytes
	lowByte = onewire_read(pin); // LS Byte
	highByte = onewire_read(pin); // MS Byte

	tt = (lowByte | highByte << 8);
	
	// temperatura dodatnia
	temperature = ((tt >> 4) & 0x7f);

	// ujemna temperatura
	if ((highByte >> 7) == 1) { 
		tt = ~tt + 1;
		temperature  = ((tt >> 4) & 0x7f);
		sign = -1;
	}
	
	// odczytanie ulamka
	fraction = ((tt & 0xf) *10) /16;

	// podanie wartosc temparatury z ulamkiem
	return sign * (temperature + fraction * 0.1);
}

