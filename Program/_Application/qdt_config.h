/* Includes ------------------------------------------------------------------*/
#include "inttypes.h"
#include <stm32f10x.h>

// define QUAD or PILOT
//#define QUAD
#define PILOT


/***
 * Pin Configuration
 */ 

/*********************** SPI - rfm12 (HW/SW) **************************/	 
// SPI interface	 
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


/*********************** SPI - SD (HW) **************************/	 
// SPI interface	 
#define SD_SPI					SPI2

// SPI - CS	 
#define SD_PORT_SS				GPIOC
#define SD_BIT_SS				GPIO_Pin_2

// SPI - DATA
#define SD_PORT_SPI				GPIOB
#define SD_BIT_SCK				GPIO_Pin_13
#define SD_BIT_MISO				GPIO_Pin_14
#define SD_BIT_MOSI				GPIO_Pin_15  
 

/*********************** LEDs (SW) *************************/	 
// LEDs port 
#define LEDS_PORT				GPIOB 
 
// LEDs pins 
#define LED_BIT_1				GPIO_Pin_12 
#define LED_BIT_2				GPIO_Pin_11 
#define LED_BIT_3				GPIO_Pin_10 
#define LED_BIT_4				GPIO_Pin_8 
#define LED_BIT_5				GPIO_Pin_9 
#define LED_BIT_6				GPIO_Pin_5 
 
 
/*********************** USART - COM (HW) *************************/	 
// GPS - USART interface 
#define COM_USART				USART1 
 
// GPS - USART port 
#define COM_PORT				GPIOA 
 
// GPS - USART pins 
#define COM_BIT_TX				GPIO_Pin_9 
#define COM_BIT_RX				GPIO_Pin_10 
 
 
/*********************** PWM - LCD (HW) *************************/	 
//#ifdef PILOT 
	
	// PWM Timer
	#define PWM_TIMER 				TIM3
	
	// PWM port 
	#define PWM_PORT				GPIOB 
 
	// PWM pins 
	#define PWM_BIT_1				GPIO_Pin_0 
//#endif 

 
/*********************** PRESSURE (HW) *************************/	  
// SPI interface	 
#define PRESSURE_I2C		I2C1

 
// PRESSURE - I2C port 
#define PRESSURE_PORT		GPIOB 
 
// PRESSURE - I2C pins 
#define PRESSURE_BIT_1		GPIO_Pin_6 
#define PRESSURE_BIT_2		GPIO_Pin_7 

 
/*********************** USART - GPS (HW) *************************/	 
//#ifdef QUAD 
	// GPS - USART interface 
	#define GPS_USART				USART2 
 
	// GPS - USART port 
	#define GPS_PORT				GPIOA 
 
	// GPS - USART pins 
	#define GPS_BIT_TX				GPIO_Pin_2 
	#define GPS_BIT_RX				GPIO_Pin_3 
//#endif 
 
 
/*********************** TEMPERATURE (SW) *************************/	  
//#ifdef QUAD 

	// GPS - USART port 
	#define TEMPERATURE_PORT		GPIOC 
 
	// GPS - USART pins 
	#define TEMPERATURE_BIT_1		GPIO_Pin_6 
	#define TEMPERATURE_BIT_2	 	GPIO_Pin_7 
	#define TEMPERATURE_BIT_3		GPIO_Pin_8 
	#define TEMPERATURE_BIT_4		GPIO_Pin_9 
	#define TEMPERATURE_BIT_5		GPIO_Pin_10 
	#define TEMPERATURE_BIT_6		GPIO_Pin_11 
 
//#endif 


/*********************** ADC - BATTERY VOLTAGE (HW) *************************/	  
//#ifdef QUAD 
 
	// ADC channels (BATTERY 1) 
	#define ADC_VOLTAGE_1_CELL_1		ADC_Channel_10 
	#define ADC_VOLTAGE_1_CELL_2	 	ADC_Channel_11 
	#define ADC_VOLTAGE_1_CELL_3		ADC_Channel_12
	 
	// ADC channels (BATTERY 2) 
	#define ADC_VOLTAGE_2_CELL_1		ADC_Channel_13 
	#define ADC_VOLTAGE_2_CELL_2	 	ADC_Channel_14 
	#define ADC_VOLTAGE_2_CELL_3		ADC_Channel_15
 
//#endif 


/*********************** ADC - BATTERY CURRENT (HW) *************************/	  
//#ifdef QUAD 
 
	// ADC channels (BATTERY 1 and 2) 
	#define ADC_CURRENT_1				ADC_Channel_8 
	#define ADC_CURRENT_2	 			ADC_Channel_9 
 
//#endif 
