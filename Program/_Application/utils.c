/**
//
//  @file	 		utils.c
//  @author 		RA Sewell
//  @brief 			Butterfly MP3 utility routines
//
//  Target(s)...: ATmega169
//
//  Compiler....: AVR-GCC 3.3.1; avr-libc 1.0
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20060304 - 1.0  - Created                                       - KS
// 
**/

#include "utils.h"
//#include "typedefs.h"
//#include <inttypes.h>



/*****************************************************************************
*
*   Function name : strLen
*
*   Returns :       Length of string
*
*   Parameters :    None
*
*   Purpose :       Find the length of a string (excluding NULL char)
*
*****************************************************************************/
uint16_t strLen(uint8_t *str)
{
   uint16_t len;

   for (len = 0; str[len] != 0x00; len++);

   return (len);
}


/*****************************************************************************
*
*   Function name : strCatChar
*
*   Returns :       None
*
*   Parameters :    None
*
*   Purpose :       Append a byte to a string buffer
*
*****************************************************************************/
void strCatChar(uint8_t *str, uint8_t byte)
{
   uint16_t len;

   len = strLen(str);
   str[len] = byte;
   str[len+1] = '\0';
}



