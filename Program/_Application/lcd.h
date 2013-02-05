#ifndef LCD_H
#define LCD_H

#include <inttypes.h>

void lcd_fill_with_color(uint8_t color);
void switch_to_screan(uint8_t screen); 

void draw_screen_0(void);
void draw_screen_1(void);
void draw_screen_2(void);
void draw_screen_3(void);
void draw_screen_4(void);
void draw_screen_5(void);

#endif // LCD_H

