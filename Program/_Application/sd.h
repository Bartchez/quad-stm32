#ifndef _SD_H
#define _SD_H

// stm32
#include <stm32f10x.h>
// pin mappings
#include "qdt_config.h"

//FATFs
#include "./../../Libraries/FATFs/src/ff.h"
#include "./../../Libraries/FATFs/src/diskio.h"


void sd_init(void);
uint8_t sd_card_detected(void);
FRESULT sd_mount(void);
FRESULT sd_open_file(const char* path);
FRESULT sd_close_file(void);

uint8_t sd_write_line(const char* path, char* text, uint8_t size, uint8_t append);

#endif /*_SD_H*/

