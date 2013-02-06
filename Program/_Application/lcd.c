//others
#include "lcd.h"
#include "lcd_ls020.h"

#include "18B20.h"
#include "MPL115A2.h"
#include "gps.h"

#include <stdio.h>
#include <string.h>

#define MIN_X 5
#define MIN_Y 5

uint8_t buffer[20];
uint8_t LCD_CURRENT_SCREEN;


////////////////////////////////////////////////////////////////////////////////////////////////////
void detect_button(void) {

	uint8_t new_screen = LCD_CURRENT_SCREEN;

	Delay_ms(10);

	if (!GPIO_ReadInputDataBit(GPIOG, BUTTON1_LEFT)) {
		if ((new_screen - 1) <= 0) {
			new_screen = 6;
		}
		
		switch_to_screan(new_screen -1);
	}

	if (!GPIO_ReadInputDataBit(GPIOG, BUTTON1_RIGHT)) {
		if ((new_screen + 1) >= 6) {
			new_screen = 0;
		}
		
		switch_to_screan(new_screen +1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void lcd_fill_with_color(uint8_t color) {
	LS020_fill_screen(color);   	  								
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void switch_to_screan(uint8_t screen) {
	if (screen != LCD_CURRENT_SCREEN) {
		LS020_clrscr();
	}	

	LCD_CURRENT_SCREEN = screen;

	if (screen == 1 ) {
		draw_screen_1();
	}
	else if (screen == 2 ) {
		draw_screen_2();
	}
	else if (screen == 3 ) {
		draw_screen_3();
	}
	else if (screen == 4 ) {
		draw_screen_4();
	}
	else if (screen == 5 ) {
		draw_screen_5();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_0(void) {
	LS020_clrscr();
	LCD_CURRENT_SCREEN = 0;
	lcd_fill_with_color(BLUE);   	  								

	// create output string
	LS020_message_centerXY(MIN_X, 90, BLUE, WHITE, "PRACA");
	LS020_message_centerXY(MIN_X, 70, BLUE, WHITE, "INZYNIERSKA");

	LS020_message_centerXY(MIN_X, 50, BLUE, WHITE, "B. LEBIODA");
	LS020_message_centerXY(MIN_X, 30, BLUE, WHITE, "E. WOJTASZEK");

	LS020_message_centerXY(MIN_X, 10, BLUE, WHITE, "PP 2012");

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_1(void) {
	lcd_fill_with_color(GREEN);   	  								
	LS020_message_centerXY(45, 110, GREEN, BLACK, "TEMPERATURA");

	// create output string
	LS020_message_centerXY(MIN_X, 90, GREEN, BLACK, "SILNIKI:");
    sprintf(buffer, " TEMP 1: %.2fC", temp_measurements[0]);    
	LS020_message_centerXY(MIN_X, 78, GREEN, BLACK, buffer);
    sprintf(buffer, " TEMP 2: %.2fC", temp_measurements[1]);    
	LS020_message_centerXY(MIN_X, 66, GREEN, BLACK, buffer);
    sprintf(buffer, " TEMP 3: %.2fC", temp_measurements[2]);    
	LS020_message_centerXY(MIN_X, 54, GREEN, BLACK, buffer);
    sprintf(buffer, " TEMP 4: %.2fC", temp_measurements[3]);    
    LS020_message_centerXY(MIN_X, 42, GREEN, BLACK, buffer);
	LS020_message_centerXY(MIN_X, 30, GREEN, BLACK, "BATERIE:");
    sprintf(buffer, " TEMP 5: %.2fC", temp_measurements[4]);    
	LS020_message_centerXY(MIN_X, 18, GREEN, BLACK, buffer);
    sprintf(buffer, " TEMP 6: %.2fC", temp_measurements[5]);    
	LS020_message_centerXY(MIN_X, 6, GREEN, BLACK, buffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_2(void) {
	lcd_fill_with_color(BLACK); 

	LS020_message_centerXY(50, 100, BLACK, WHITE, "CISNIENIE");
    sprintf(buffer, "%.3f hPa", mpl115a2_pressure);    
	LS020_message_centerXY(MIN_X + 30, 70, BLACK, WHITE, buffer);
    sprintf(buffer, "%.3f C", mpl115a2_temp);    
	LS020_message_centerXY(MIN_X + 30, 55, BLACK, WHITE, buffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_3(void) {
	lcd_fill_with_color(WHITE); 
	LS020_message_centerXY(75,110,WHITE,BLACK,"GPS");

    sprintf(buffer, " CZAS: %s", gps_time_tab);    
	LS020_message_centerXY(MIN_X, 90, WHITE,BLACK, buffer);
    sprintf(buffer, " SZER. GEO.: %s", gps_latitude_tab);    
	LS020_message_centerXY(MIN_X, 78, WHITE,BLACK, buffer);
    sprintf(buffer, " DLU. GEO: %s", gps_longitude_tab);    
	LS020_message_centerXY(MIN_X, 66, WHITE,BLACK, buffer);
    sprintf(buffer, " PREDKOSC: %s", gps_speed_tab);    
	LS020_message_centerXY(MIN_X, 54, WHITE,BLACK, buffer);
    sprintf(buffer, " KIERUNEK: %s", gps_direction_tab);    
	LS020_message_centerXY(MIN_X, 42, WHITE,BLACK, buffer);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_4(void) {
	lcd_fill_with_color(BLUE);   	  								

	LS020_message_centerXY(50, 110, BLUE, WHITE, "BATERIA 1");
	LS020_message_centerXY(MIN_X, 90, BLUE, WHITE, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 78, BLUE, WHITE, " NAPIECIE: %d,%03dV");
    LS020_message_centerXY(MIN_X, 66, BLUE, WHITE, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 54, BLUE, WHITE, " NATEZENIE: %d,%03dA");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void draw_screen_5(void) {
	lcd_fill_with_color(YELLOW); 

	LS020_message_centerXY(50, 110, YELLOW, BLACK, "BATERIA 2");
	LS020_message_centerXY(MIN_X, 90, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 78, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 66, YELLOW, BLACK, " NAPIECIE: %d,%03dV");
	LS020_message_centerXY(MIN_X, 54, YELLOW, BLACK, " NATEZENIE: %d,%03dA");
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
