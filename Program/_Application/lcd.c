//others
#include "lcd.h"
#include "lcd_ls020.h"
#include "18B20.h"

#include <stdio.h>
#include <string.h>

#define MIN_X 5
#define MIN_Y 5

uint8_t buffer[100];

////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_fill_with_color(uint8_t color) {
	LS020_DrawRect(128, 2, 2, 173, color);   	  								
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_1(void) {
	lcd_fill_with_color(BLACK);   	  								
	LS020_message_centerXY(45, 110, GREEN, BLACK, "TEMPERATURA");

	// create output string
	LS020_message_centerXY(MIN_X, 90, GREEN, BLACK, "SILNIKI:");
    sprintf(buffer, "%.2f", temp_measurements[0]);    
	LS020_message_centerXY(MIN_X, 78, GREEN, BLACK, " TEMP1: %d,%02dC");
    sprintf(buffer, "%.2f", temp_measurements[1]);    
	LS020_message_centerXY(MIN_X, 66, GREEN, BLACK, " TEMP2: %d,%03dC");
    sprintf(buffer, "%.2f", temp_measurements[2]);    
	LS020_message_centerXY(MIN_X, 54, GREEN, BLACK, " TEMP3: %d,%03dC");
    sprintf(buffer, "%.2f", temp_measurements[3]);    
    LS020_message_centerXY(MIN_X, 42, GREEN, BLACK, " TEMP4: %d,%03dC");
	LS020_message_centerXY(MIN_X, 30, GREEN, BLACK, "BATERIE:");
    sprintf(buffer, "%.2f", temp_measurements[4]);    
	LS020_message_centerXY(MIN_X, 18, GREEN, BLACK, " TEMP5: %d,%03dC");
    sprintf(buffer, "%.2f", temp_measurements[5]);    
	LS020_message_centerXY(MIN_X, 6, GREEN, BLACK, " TEMP6: %d,%03dC");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_2(void) {
	lcd_fill_with_color(BLACK);   	  								

	LS020_message_centerXY(50, 110, BLUE, WHITE, "BATERIA 1");
	LS020_message_centerXY(MIN_X, 90, BLUE, WHITE, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 78, BLUE, WHITE, " NAPIECIE: %d,%03dV");
    LS020_message_centerXY(MIN_X, 66, BLUE, WHITE, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 54, BLUE, WHITE, " NATEZENIE: %d,%03dA");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_3(void) {
	lcd_fill_with_color(BLACK); 
	LS020_message_centerXY(50, 110, YELLOW, BLACK, "BATERIA 2");
	LS020_message_centerXY(MIN_X, 90, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 78, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 66, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 54, YELLOW, BLACK, " NATEZENIE: %d,%03dA");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_4(void) {
	lcd_fill_with_color(RED); 
	LS020_message_centerXY(50, 110, BLACK, WHITE, "CSNIENIE");
	LS020_message_centerXY(MIN_X, 90, BLACK, WHITE, " CSNIENIE: %d,%03dhPa");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_5(void) {
	lcd_fill_with_color(BLACK); 
	LS020_message_centerXY(75,110,WHITE,BLACK,"GPS");
			
	LS020_message_centerXY(MIN_X, 90, WHITE,BLACK, " CZAS: %d,%03d");
	LS020_message_centerXY(MIN_X, 78, WHITE,BLACK, " WSPOL X: %d,%03d");
	LS020_message_centerXY(MIN_X, 66, WHITE,BLACK, " WSPOL Y: %d,%03d");
	LS020_message_centerXY(MIN_X, 54, WHITE,BLACK, " KIERUNEK: %d,%03d");
	LS020_message_centerXY(MIN_X, 42, WHITE,BLACK, " PREDKOSC: %d,%03dC");
}
/*
void LCD_Menu(void) {
	volatile unsigned long int i;

	switch (LCD){

		case MENU_1 : 
		{
			LS020_DrawRect(128,2,2,173,BLACK);   	  								
			LS020_message_centerXY(45,110,GREEN,BLACK,"TEMPERATURA");

			LS020_message_centerXY(5,90,GREEN,BLACK,"SILNIKI:");
			LS020_message_centerXY(5,78,GREEN,BLACK," TEMP1: %d,%02dC");
		    LS020_message_centerXY(5,66,GREEN,BLACK," TEMP2: %d,%03dC");
			LS020_message_centerXY(5,54,GREEN,BLACK," TEMP3: %d,%03dC");
            LS020_message_centerXY(5,42,GREEN,BLACK," TEMP4: %d,%03dC");
			LS020_message_centerXY(5,30,GREEN,BLACK,"BATERIE:");
			LS020_message_centerXY(5,18,GREEN,BLACK," TEMP5: %d,%03dC");
			LS020_message_centerXY(5,6,GREEN,BLACK," TEMP6: %d,%03dC");
									
			for (i=0;i<10000ul;i++);
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)) {     //MENU_2
				GPIO_SetBits(GPIOB, GPIO_Pin_12);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(BLUE);
				LCD = 1;
										
			}
			
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {     //MENU_0
				GPIO_ResetBits(GPIOB, GPIO_Pin_12);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(WHITE);
				LCD=4;
										
			}
		} break;

		case MENU_2 : { 
			LS020_DrawRect(128,2,2,173,BLACK); 
			LS020_message_centerXY(50,110,BLUE,WHITE,"BATERIA 1");
			LS020_message_centerXY(5,90,BLUE,WHITE," NAPIECIE: %d,%03dV");
			LS020_message_centerXY(5,78,BLUE,WHITE," NAPIECIE: %d,%03dV");
		    LS020_message_centerXY(5,66,BLUE,WHITE," NAPIECIE: %d,%03dV");
			LS020_message_centerXY(5,54,BLUE,WHITE," NATEZENIE: %d,%03dA");
            
			for (i=0;i<10000ul;i++);
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) && LCD == 1) {     //MENU_3
				GPIO_SetBits(GPIOB, GPIO_Pin_11);
				
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(YELLOW);
				LCD=2;	
			}
			
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){     //MENU_1
				GPIO_ResetBits(GPIOB, GPIO_Pin_11);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(GREEN);
				LCD=0;
			}
			
		} break;

		case MENU_3 : {
			LS020_DrawRect(128,2,2,173,BLACK); 
			LS020_message_centerXY(50,110,YELLOW,BLACK,"BATERIA 2");
			LS020_message_centerXY(5,90,YELLOW,BLACK," NAPIECIE: %d,%03dV");
			LS020_message_centerXY(5,78,YELLOW,BLACK," NAPIECIE: %d,%03dV");
			LS020_message_centerXY(5,66,YELLOW,BLACK," NAPIECIE: %d,%03dV");
			LS020_message_centerXY(5,54,YELLOW,BLACK," NATEZENIE: %d,%03dA");
									
			for (i=0;i<10000ul;i++);
			
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){     //MENU_4
				GPIO_SetBits(GPIOB, GPIO_Pin_10);
			
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(BLACK);
				LCD=3;

			}

			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){     //MENU_2
				GPIO_ResetBits(GPIOB, GPIO_Pin_10);

				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(BLUE);
				LCD=1;
			}
			
		} break;
      	
		case MENU_4 : {
			LS020_DrawRect(128,2,2,173,RED);
			LS020_message_centerXY(50,110,BLACK,WHITE,"CSNIENIE");
			LS020_message_centerXY(5,90,BLACK,WHITE," CSNIENIE: %d,%03dhPa");
			
			for (i=0;i<10000ul;i++);
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){     //MENU_1
				GPIO_SetBits(GPIOB, GPIO_Pin_9);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(WHITE);
				LCD=4;
			}
		
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){     //MENU_3
		
				GPIO_ResetBits(GPIOB, GPIO_Pin_9);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(YELLOW);
				LCD=2;
			}
		} break;
		
		case MENU_5 : {
			LS020_DrawRect(128,2,2,173,BLACK);
			LS020_message_centerXY(75,110,WHITE,BLACK,"GPS");
			
			LS020_message_centerXY(5,90,WHITE,BLACK," CZAS: %d,%03d");
			LS020_message_centerXY(5,78,WHITE,BLACK," WSPOL X: %d,%03d");
			LS020_message_centerXY(5,66,WHITE,BLACK," WSPOL Y: %d,%03d");
			LS020_message_centerXY(5,54,WHITE,BLACK," KIERUNEK: %d,%03d");
			LS020_message_centerXY(5,42,WHITE,BLACK," PREDKOSC: %d,%03dC");
			
			for (i=0;i<10000ul;i++);
			if (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){     //MENU_1
				GPIO_SetBits(GPIOB, GPIO_Pin_8);
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(GREEN);
				LCD=0;
										
			}
			
			if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){     //MENU_4
				GPIO_ResetBits(GPIOB, GPIO_Pin_8);
				
				for (i=0;i<1500000ul;i++);
				LS020_fill_screen(BLACK);
				LCD=3;
										
			}
		} break;

      	default : LCD = MENU_1;
    }
}
  */
