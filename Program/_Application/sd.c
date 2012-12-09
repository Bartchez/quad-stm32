#include "sd.h"

//others
#include <stdio.h>
#include <string.h>

uint8_t sd_write_line(const char* path, char* text, uint8_t size, uint8_t append) {

	FIL file;
	FRESULT fresult;
	UINT savedBytes = 0;

	// open file
	fresult = f_open(&file, path, FA_WRITE | FA_OPEN_ALWAYS);
	
	//exit if error
    if (fresult) {
		// return error code
		return 1;
	}

	if (append & 0x1 == 1) {
		fresult = f_lseek(&file, file.fsize);
    } 
	
	else {
		fresult = f_lseek(&file, 0);
        fresult = f_truncate(&file);
	}
	
	// write text fo file
    fresult = f_write(&file, text, 6, &savedBytes);	           
	
	// if any problems
    if (fresult) {
		// close file
	 	fresult = f_close (&file);

		// return error code
		return 2;
	}

	// close file
 	fresult = f_close (&file);

	// return no error
	return 0;
}

