#ifndef _18B20_H
#define _18B20_H

// stm32
#include <stm32f10x.h>
// pin mappings
#include "qdt_config.h"

#define STARTCONVO      0x44 // Initalises slaves
#define COPYSCRATCH     0x48 // Copy EEPROM
#define READSCRATCH     0xBE // Read EEPROM
#define WRITESCRATCH    0x4E // Write to EEPROM
#define TEMPCOMMAND     0xBE // Request temperature
#define RECALLSCRATCH   0xB8 // Reload from last known
#define SKIPROM			0xCC // Skip ROM

// communication with sensor
void onewire_write(uint8_t data, uint16_t pin);
uint8_t onewire_reset(uint16_t pin); // if 0 sensor is presence, 1 not presence
uint8_t onewire_read(uint16_t pin); // return read byte

// change GPIO mode
void Onewire_OUT_PULL_UP(uint16_t pin);
void Onewire_OUT_FLOATING(uint16_t pin);
void Onewire_IN_FLOATING(uint16_t pin);
void Onewire_IN_PULL_UP(uint16_t pin);

// read temp from GPIO pin, GPIO port is definied in OneWire.h
float ds18b20_read_temp(uint16_t pin);

// helpers
void _delay_us(u8 us);

// keep measured temperatures from 6 sensors
extern volatile float temp_measurements[6];

// keep measured temperatures from 6 sensors as array of chars
extern volatile char temp_measurements_string[50];

// read temp values from 6 sensors and save to temp_measurements array
void ds18b20_read_temps(void);
#endif /*_18B20_H*/

