/*******************************************************************************
* File Name      : S65DISP.h
* Description    : Header for S65 Display Code
*******************************************************************************/
#ifndef __LS020_H
#define __LS020_H

//#include "stm32f10x_lib.h"
//#include "main.h"
#include "qdt_config.h"

#define uint8_t  unsigned char
#define  int8_t    signed char
#define uint16_t unsigned int
#define  int16_t   signed int
#define uint32_t unsigned long
#define  int32_t   signed long


/* Macros for toggeling the GPIOs of the S65 Disp */
#define DISP_W 132
#define DISP_H 176
#define CHAR_H 14
#define CHAR_W 8
#define TEXT_COL 16
#define TEXT_ROW 12

#define BLACK		0x0000       
#define WHITE       0xFFFF       
#define RED         0xF800       
#define GREEN       0x07E0       
#define BLUE        0x001F       
#define YELLOW      0xFFC0       
#define MAGENTA     0xF81F       
#define CYAN        0x07FF       
#define GRAY        0x4108       
#define SILVER      0xF408       
#define GOLD        0xF404

#define RS_SET GPIO_WriteBit(LCD_PORT_LS020, LCD_PIN_RS, Bit_SET);
#define RS_CLR GPIO_WriteBit(LCD_PORT_LS020, LCD_PIN_RS, Bit_RESET);

#define RESET_SET GPIO_WriteBit(LCD_PORT_LS020, LCD_PIN_RESET, Bit_SET);
#define RESET_CLR GPIO_WriteBit(LCD_PORT_LS020, LCD_PIN_RESET, Bit_RESET);

#define CS_SET GPIO_WriteBit(LCD_PORT_SS, LCD_BIT_SS, Bit_SET);
#define CS_CLR GPIO_WriteBit(LCD_PORT_SS, LCD_BIT_SS, Bit_RESET);

void LS020_DrawPixelXY(uint8_t x, uint8_t y, uint16_t color);
void LS020_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
void LS020_DrawRect(uint8_t sr1,uint8_t sr2,uint8_t szer,uint8_t wys, uint8_t color);
void LS020_DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint16_t color);
void LS020_clrscr(void);              																					/* Clear LCD */
void LS020_fill_screen(uint16_t color);          																/* Fill LCD by Color */
void LS020_DrawPixelXY(uint8_t x, uint8_t y, uint16_t color);
void LS020_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint16_t color);
void LS020_DrawCircle(uint8_t xCenter, uint8_t yCenter, uint8_t radius, uint16_t color);
void LS020_put_char(uint8_t x, uint8_t y, uint16_t textcolor, uint16_t backcolor, char c);
void LS020_message_centerP(uint8_t line,const char* string);
void LS020_message_center(uint8_t line,const char* string);
void LS020_message_centerXY(uint8_t x,uint8_t y, uint16_t textcolor, uint16_t backcolor, const char* string);
void LS020_put_char_maxXY(uint8_t x, uint8_t y, uint16_t textcolor, uint16_t backcolor, uint8_t size, const char* c);
 
void LS020_wrdat(uint16_t data);          																			/* sends  16Bit Data through SPI1 */
void LS020_wrcmd(uint16_t cmd);           																			/* sends a 16Bit command through SPI1 */

void S65_init(void);                  																					/* Initialise the S65 Display */

void S65_FillScreen(uint16_t color);       																			/* Fills the Screen with a color */
void S65_DrawBitmap(uint8_t *bitmap);      																			/* Draws a Bitmap to the Screen */

#endif /* __LS020_H */
