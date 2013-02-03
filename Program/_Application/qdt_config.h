#ifndef _QDT_CONFIG_H
#define _QDT_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "inttypes.h"
#include <stm32f10x.h>

// define QUAD or PILOT
#define QUAD
//#define PILOT


/***
 * Pin Configuration
 */ 

/*********************** SPI - rfm12 (HW/SW) **************************/	 
// SPI interface	 
#ifdef QUAD	 
	#define RFM12_SPI				SPI1

	// SPI - CS	 
	#define RFM12_PORT_SS			GPIOA
	#define RFM12_BIT_SS			GPIO_Pin_8

	// SPI - DATA
	#define RFM12_PORT_SPI			GPIOA
	#define RFM12_BIT_SCK			GPIO_Pin_5
	#define RFM12_BIT_MISO			GPIO_Pin_6
	#define RFM12_BIT_MOSI			GPIO_Pin_7  

	// SPI - INT 
	#define RFM12_PORT_INT			GPIOA
	#define RFM12_BIT_INT			GPIO_Pin_9 
#else
	#define RFM12_SPI				SPI2

	// SPI - CS	 
	#define RFM12_PORT_SS			GPIOB
	#define RFM12_BIT_SS			GPIO_Pin_12

	// SPI - DATA
	#define RFM12_PORT_SPI			GPIOB
	#define RFM12_BIT_SCK			GPIO_Pin_13
	#define RFM12_BIT_MISO			GPIO_Pin_14
	#define RFM12_BIT_MOSI			GPIO_Pin_15  

	// SPI - INT 
	#define RFM12_PORT_INT			GPIOD
	#define RFM12_BIT_INT			GPIO_Pin_8 
#endif

/*********************** SPI - SD (HW) **************************/	 
// SPI interface	 
#define SD_SPI					SPI1

// SPI - CS	 
#define SD_PORT_SS				GPIOA
#define SD_BIT_SS				GPIO_Pin_4

// SPI - DETECT	 
#define SD_PORT_DETECT			GPIOA
#define SD_BIT_DETECT			GPIO_Pin_8

// SPI - DATA
#define SD_PORT_SPI				GPIOA
#define SD_BIT_SCK				GPIO_Pin_5
#define SD_BIT_MISO				GPIO_Pin_6
#define SD_BIT_MOSI				GPIO_Pin_7  


/*********************** SPI - LCD (HW) **************************/	 
// SPI interface	 
#define LCD_SPI					SPI3

// SPI - CS	 
#define LCD_PORT_SS				GPIOC
#define LCD_BIT_SS				GPIO_Pin_1

// SPI - DATA
//#define LCD_PORT_SPI			GPIOB
//#define LCD_BIT_SCK			GPIO_Pin_3
//#define LCD_BIT_MOSI			GPIO_Pin_5  
#define LCD_PORT_SPI			GPIOC
#define LCD_BIT_SCK				GPIO_Pin_10
#define LCD_BIT_MOSI			GPIO_Pin_12  

// S65 LCD Config RS PIN
#define LCD_PORT_LS020 			GPIOC
#define LCD_PIN_RS  			GPIO_Pin_3

//S65 LCD Config RESET PIN
#define LCD_PIN_RESET  			GPIO_Pin_2


/*********************** LEDs (SW) *************************/	 
#ifdef QUAD	 
	// LEDs port 
	#define LEDS_PORT				GPIOB 
 
	// LEDs pins 
	#define LED_BIT_1				GPIO_Pin_5
	#define LED_BIT_2				GPIO_Pin_8 
	#define LED_BIT_3				GPIO_Pin_9 
	#define LED_BIT_4				GPIO_Pin_10 
	#define LED_BIT_5				GPIO_Pin_11 
	#define LED_BIT_6				GPIO_Pin_12 
#else
	// LEDs port 
	#define LEDS_PORT				GPIOE 
 
	// LEDs pins 
	#define LED_BIT_1				GPIO_Pin_7
	#define LED_BIT_2				GPIO_Pin_8 
	#define LED_BIT_3				GPIO_Pin_9 
	#define LED_BIT_4				GPIO_Pin_10 
	#define LED_BIT_5				GPIO_Pin_11 
	#define LED_BIT_6				GPIO_Pin_12 
#endif 
 
/*********************** USART - COM (HW) *************************/	 // TEGO BRAK!
// GPS - USART interface 
#define COM_USART				USART1 
 
// GPS - USART port 
#define COM_PORT				GPIOA 
 
// GPS - USART pins 
#define COM_BIT_TX				GPIO_Pin_9 
#define COM_BIT_RX				GPIO_Pin_10 
 
 
/*********************** PWM - LCD (HW) *************************/	 
// PWM Timer
#define PWM_TIMER 				TIM3 // ???
	
// PWM port 
#define PWM_PORT				GPIOB 
 
// PWM pins 
#define PWM_BIT_1				GPIO_Pin_0 

 
/*********************** PRESSURE (HW) *************************/	  
// SPI interface	 
#define PRESSURE_I2C		I2C1

 
// PRESSURE - I2C port 
#define PRESSURE_PORT		GPIOB 
 
// PRESSURE - I2C pins 
#define PRESSURE_BIT_1		GPIO_Pin_6 
#define PRESSURE_BIT_2		GPIO_Pin_7 

 
/*********************** USART - GPS (HW) *************************/	 
// GPS - USART interface 
#define GPS_USART				USART2 
 
// GPS - USART port 
#define GPS_PORT				GPIOA
 
// GPS - USART pins 
#define GPS_BIT_TX				GPIO_Pin_2 
#define GPS_BIT_RX				GPIO_Pin_3 
 
 
/*********************** TEMPERATURE (SW) *************************/	  
// GPS - USART port 
#define TEMPERATURE_PORT		GPIOC 
 
// GPS - USART pins 
#define TEMPERATURE_BIT_1		GPIO_Pin_6 
#define TEMPERATURE_BIT_2	 	GPIO_Pin_7 
#define TEMPERATURE_BIT_3		GPIO_Pin_8 
#define TEMPERATURE_BIT_4		GPIO_Pin_9 
#define TEMPERATURE_BIT_5		GPIO_Pin_10 
#define TEMPERATURE_BIT_6		GPIO_Pin_11 


/*********************** ADC - BATTERY VOLTAGE (HW) *************************/	  
// ADC channels (BATTERY 1) 
#define ADC_VOLTAGE_1_CELL_1		ADC_Channel_10 
#define ADC_VOLTAGE_1_CELL_2	 	ADC_Channel_11 
#define ADC_VOLTAGE_1_CELL_3		ADC_Channel_12
	 
// ADC channels (BATTERY 2) 
#define ADC_VOLTAGE_2_CELL_1		ADC_Channel_13 
#define ADC_VOLTAGE_2_CELL_2	 	ADC_Channel_14 
#define ADC_VOLTAGE_2_CELL_3		ADC_Channel_15


/*********************** ADC - BATTERY CURRENT (HW) *************************/	  
// ADC channels (BATTERY 1 and 2) 
#define ADC_CURRENT_1				ADC_Channel_8 
#define ADC_CURRENT_2	 			ADC_Channel_9 


// TODO

/*********************** BUTTONS(HW) *************************/	  
// GPS - USART port 
#define BUTTON_PORT					GPIOG 

#define BUTTON1_UP					GPIO_Pin_6 
#define BUTTON1_DOWN				GPIO_Pin_7 
#define BUTTON1_LEFT				GPIO_Pin_11 
#define BUTTON1_RIGHT				GPIO_Pin_13 
#define BUTTON1_SELECT				GPIO_Pin_14 

#endif /* _QDT_CONFIG_H */ 
