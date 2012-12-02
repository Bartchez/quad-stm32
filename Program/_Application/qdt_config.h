/* Includes ------------------------------------------------------------------*/
#include "inttypes.h"
#include <stm32f10x.h>

// define QUAD or PILOT
#define QUAD
//#define PILOT


/***
 * Pin Configuration
 */

/*********************** SPI - rfm12 **************************/	 
// SPI interface	 
#define RFM12_SPI				SPI1
	 
// SPI - CS	 
#define RFM12_PORT_SS			GPIOB
#define RFM12_BIT_SS			GPIO_Pin_14

// SPI - DATA
#define RFM12_PORT_SPI			GPIOA
#define RFM12_BIT_SCK			GPIO_Pin_5
#define RFM12_BIT_MISO			GPIO_Pin_6
#define RFM12_BIT_MOSI			GPIO_Pin_7 

// SPI - INT
#define RFM12_PORT_INT			GPIOC
#define RFM12_BIT_INT			GPIO_Pin_0


/*********************** LEDs *************************/	 
// LEDs port
#define LEDS_PORT				GPIOB

// LEDs pins
#define LED_BIT_1				GPIO_Pin_15
#define LED_BIT_2				GPIO_Pin_11
#define LED_BIT_3				GPIO_Pin_13
#define LED_BIT_4				GPIO_Pin_12


/*********************** USART - COM *************************/	 
// GPS - USART interface
#define COM_USART				USART1

// GPS - USART port
#define COM_PORT				GPIOA

// GPS - USART pins
#define COM_BIT_TX				GPIO_Pin_9
#define COM_BIT_RX				GPIO_Pin_10


/*********************** PWM - LCD *************************/	 
#ifdef PILOT
	// PWM port
	#define PWM_PORT				GPIOB

	// PWM pins
	#define PWM_BIT_1				GPIO_Pin_8
#endif


/*********************** USART - GPS *************************/	 
#ifdef QUAD
	// GPS - USART interface
	#define GPS_USART				USART2

	// GPS - USART port
	#define GPS_PORT				GPIOA

	// GPS - USART pins
	#define GPS_BIT_TX				GPIO_Pin_2
	#define GPS_BIT_RX				GPIO_Pin_3
#endif


/*********************** TEMPERATURE *************************/	 
#ifdef QUAD

	// GPS - USART port
	#define TEMPERATURE_PORT		GPIOC

	// GPS - USART pins
	#define TEMPERATURE_BIT_1		GPIO_Pin_3
	#define TEMPERATURE_BIT_2	 	GPIO_Pin_4
	#define TEMPERATURE_BIT_3		GPIO_Pin_5
	#define TEMPERATURE_BIT_4		GPIO_Pin_6
	#define TEMPERATURE_BIT_5		GPIO_Pin_7
	#define TEMPERATURE_BIT_6		GPIO_Pin_8

#endif


/***
 * Communicatin commands
 */

#define RFM12_TEMPERATURE_CMD	0x1A
#define RFM12_PRESSURE_CMD		0x2A
#define RFM12_GPS_CMD			0x3A
#define RFM12_TENSION_CMD		0x4A
#define RFM12_CURRENT_CMD		0x5A

