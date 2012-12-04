 
/* Includes ------------------------------------------------------------------*/ 
#include "stm32f10x.h" 
 
// Configurations 
#include "qdt_config.h" 
#include "core_cm3.h" 
 
// Own libraries 
#include "gps.h" 
#include "rfm12.h" 
#include "18B20.h" 
#include "MPL115A2.h" 
#include "rfm12_controller.h" 
 
/* Private typedef -----------------------------------------------------------*/ 
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/ 
/* Private variables ---------------------------------------------------------*/ 
/* Private function prototypes -----------------------------------------------*/ 
void RCC_Configuration(void); 
void TIM_Configuration(void); 
void SPI_Configuration(void); 
void GPIO_Configuration(void); 
void NVIC_Configuration(void); 
void EXTI_Configuration(void); 
void USART_Configuration(void); 
void SysTick_Configuration(void); 
void I2C_Configuration(void); 
 
/* Private functions ---------------------------------------------------------*/ 
void Delay(__IO uint32_t nCount); 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
int main(void)  
{ 
	uint8_t ret = 0; 
	 
	char test[120]; 
 
	/* NVIC Configuration */ 
	NVIC_Configuration(); 
 
	/* System Clocks Configuration */ 
	RCC_Configuration(); 
 
	/* GPIO Configuration */ 
	GPIO_Configuration(); 
 
	/* USART Configuration */ 
	USART_Configuration(); 
 
	/* SPI Configuration */ 
	SPI_Configuration(); 
 
	/* EXTI Configuration */ 
	EXTI_Configuration(); 
 
	/* TIM Configuration */ 
	TIM_Configuration(); 
 
	/* I2C Configuration */ 
	I2C_Configuration(); 
 
	/* SysTick Configuration */ 
	SysTick_Configuration(); 
 
	/* Init rfm12 module */ 
	rf12_init();   
 
	/* Init GPS sensor */ 
	gps_init(); 
 
	/* Init send/recive rfm12 controller */ 
	rf12_controller_init(); 
 
	/* Init send/recive rfm12 controller */ 
	rf12_controller_init(); 
 
	// odcekaj po konfuguracji rfm12 
	Delay(0x0fffff); 
 
	while (1)  
	{		  
 
		// blink LED 
		GPIO_ResetBits(LEDS_PORT, LED_BIT_1); //LED8 ON 
    	GPIO_SetBits(LEDS_PORT, LED_BIT_2);   //LED9 OFF 
    	Delay(0x5FFFF); 
		GPIO_SetBits(LEDS_PORT, LED_BIT_1);   //LED9 ON 
    	GPIO_ResetBits(LEDS_PORT, LED_BIT_2); //LED8 OFF 
    	Delay(0x5FFFF); 
 
#ifdef QUAD		  

		// read pressure values sensor and save to mpl115a2_pressure ivar 
		mpl115a2_read_pressure(); 

		// read temp values from 6 sensors and save to temp_measurements array 
		ds18b20_read_temps(); 

#endif 
 
#ifdef PILOT		 
 
		if( !(rf12_rx || rf12_tx || rf12_new) )  rf12_rxstart(); 
 
		if( rf12_new )	{ 
 
			ret = rf12_rxfinish(test);	// sprawdY czy odebrano kompletn1 ramke 
 
			if(ret > 0 && ret < 254) {	// brak b3edów CRC - odebrana ramka 
				printf(test);		// wyolij odebrane dane do terminala PC 

				test[16]=0;				// przytnij dane do 16 znaków ASCII 
			} 
			else 
				 
			if(!ret) {					// wyst1pi3 b31d CRC lub d3ugooci ramki 
				printf("\r\n"); 
				printf("--------------------\r\n"); 
				printf("|  CRC error !!!   |\r\n"); 
				printf("--------------------\r\n"); 
				printf("\r\n"); 
			} 
		} 
#endif 
 
	}; 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void GPIO_Configuration(void)  
{ 
 
	GPIO_InitTypeDef  GPIO_InitStructure;  
 
	/* Configure all unused GPIO port pins in Analog Input mode (floating input 
     trigger OFF), this will reduce the power consumption and increase the device 
     immunity against EMI/EMC *************************************************/ 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
 
	/* GPIO for COM */ 
 
	// USART1 - TX 
    GPIO_InitStructure.GPIO_Pin = COM_BIT_TX;	          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(COM_PORT, &GPIO_InitStructure);		    
 
 
	// USART1 - RX 
	GPIO_InitStructure.GPIO_Pin = COM_BIT_RX;	         
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(COM_PORT, &GPIO_InitStructure); 
 
 
	/* GPIO for LEDs */ 
 
	//LEDs 
	GPIO_InitStructure.GPIO_Pin = LED_BIT_1 | LED_BIT_2 | LED_BIT_3 | LED_BIT_4; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(LEDS_PORT, &GPIO_InitStructure);  
 
 
	/* GPIO for rfm12 */ 
 
	// SPI - CS 
	GPIO_InitStructure.GPIO_Pin = RFM12_BIT_SS; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(RFM12_PORT_SS, &GPIO_InitStructure); 
 
	// SPI - INT 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
 
	// SPI - SCK, MOSI 
	GPIO_InitStructure.GPIO_Pin = RFM12_BIT_SCK | RFM12_BIT_MOSI | RFM12_BIT_MISO; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(RFM12_PORT_SPI, &GPIO_InitStructure); 
 
#ifdef PILOT 
	/* GPIO for LCD */ 
 
	//PWM 
	GPIO_InitStructure.GPIO_Pin = PWM_BIT_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(PWM_PORT, &GPIO_InitStructure); 
#endif 
 
 
#ifdef QUAD 
	/* GPIO for I2C */ 

    GPIO_InitStructure.GPIO_Pin =  PRESSURE_BIT_1 | PRESSURE_BIT_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
    GPIO_Init(PRESSURE_PORT, &GPIO_InitStructure); 

 
	/* GPIO for GPS */ 
 
	// USART2 - TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Pin = GPS_BIT_TX; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPS_PORT, &GPIO_InitStructure); 
 
	// USART2 - RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = GPS_BIT_RX; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPS_PORT, &GPIO_InitStructure); 
 
 
	/* GPIO for TEMPERATURE */ 
 
	// TEMPERATURE 
	GPIO_InitStructure.GPIO_Pin = TEMPERATURE_BIT_1 | TEMPERATURE_BIT_2 | TEMPERATURE_BIT_3 | TEMPERATURE_BIT_4 |TEMPERATURE_BIT_5 | TEMPERATURE_BIT_6;	          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStructure);	 
#endif 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void RCC_Configuration(void)  
{ 
 
	ErrorStatus HSEStartUpStatus;  //zmienna opisujaca rezultat uruchomienia HSE 
 
	/* RCC system reset(for debug purpose) */ 
	RCC_DeInit(); 
 
	/* Enable HSE */ 
	RCC_HSEConfig(RCC_HSE_ON); 
 
	/* Wait till HSE is ready */ 
	HSEStartUpStatus = RCC_WaitForHSEStartUp(); 
	 
	if (HSEStartUpStatus == SUCCESS) 
 	{ 
    	/* Enable Prefetch Buffer */ 
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 
 
		/* Flash 2 wait state */ 
		FLASH_SetLatency(FLASH_Latency_2); 
 
    	/* HCLK = SYSCLK */ 
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
 
    	/* PCLK2 = HCLK */ 
		RCC_PCLK2Config(RCC_HCLK_Div1); 
 
    	/* PCLK1 = HCLK/4 */ 
		RCC_PCLK1Config(RCC_HCLK_Div2); 
 
    	/* PLLCLK = 8MHz * 9 = 72 MHz */ 
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
 
    	/* Enable PLL */ 
		RCC_PLLCmd(ENABLE); 
 
    	/* Wait till PLL is ready */ 
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {} 
 
    	/* Select PLL as system clock source */ 
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
 
    	/* Wait till PLL is used as system clock source */ 
		while (RCC_GetSYSCLKSource() != 0x08) {} 
	} 
 
	/* SPI1 clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
 
	/* GPIO A/B/C clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 
 
	/* AFIO clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
 
	/* USART1 clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
 
#ifdef QUAD 
	/* USART2 clock enable */ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
 
	/* TIM4 clock enable */	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
 
	/* I2C clock enable */	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE); 
#endif 
 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void NVIC_Configuration(void)  
{ 
	NVIC_InitTypeDef NVIC_InitStructure; 
 
#ifdef  VECT_TAB_RAM 
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */ 
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); 
#endif 
 
 
	/* NVIC for rfm12 */ 
 
	//Konfiguracja NVIC - ustawienia priorytetow przerwania EXT0 
	//Wybor modelu grupowania przerwan 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 
	 
	//Wybor konfigurowanego IRQ 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 
	 
	//Priorytet grupowy 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	//Podpriorytet 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	//Wlaczenie obslugi IRQ 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
 
 
	/* NVIC for GPS */ 
 
#ifdef QUAD 
	// Wybranie grupy priorytetów  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  
 
	// Wlacz przerwanie od USART2  
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
#endif 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void EXTI_Configuration(void)  
{ 
 
	EXTI_InitTypeDef EXTI_InitStructure; 
 
	/* EXTI for rfm12 */ 
 
	//Ustawienia zrodla przerwania 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource0); 
	 
	//Konfiguracja przerwania EXTI0 na linie 1 
	//Wybor lini 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; 
	 
	//Ustawienie generowania przerwania 
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
	 
	//Wyzwalanie zboczem opadajacym 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	 
	//Wlaczenie przerwania 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; 
	EXTI_Init(&EXTI_InitStructure);	 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void SPI_Configuration(void) { 
 
	SPI_InitTypeDef SPI_InitStructure;  
 
	/* SPI for rfm12 */ 
 
	// Konfiguracja SPI 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(RFM12_SPI, &SPI_InitStructure); 
	SPI_Cmd(RFM12_SPI, ENABLE);  
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void USART_Configuration(void) { 
	USART_InitTypeDef USART_InitStructure;  
 
 	// dlugosc slowa = 8 bitow 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	// 1 bit stopu 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	// Brak kontroli parzystosci 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	// Kontrola przeplywu danych wylaczona 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	//Tryb pracy (Rx - odbior, Tx - nadawanie) 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
 
 
	/* USART for GPS */ 
 
#ifdef QUAD 
	// reset 
	USART_DeInit(GPS_USART); 
 	// predkosc transmisji = 9600 
 	USART_InitStructure.USART_BaudRate = 9600; 
 
	// Init - USART 
	USART_Init(GPS_USART, &USART_InitStructure); 
	USART_ClearFlag(GPS_USART, USART_FLAG_TC); 
	// Wlaczenie przerwan wywolywanych przez USART2 
	USART_ITConfig(GPS_USART, USART_IT_RXNE, ENABLE); 
	// Wlaczenie USART 
	USART_Cmd(GPS_USART, ENABLE); 
#endif 
 
	/* USART for COM */ 
 
	// reset 
	USART_DeInit(COM_USART); 
 	// predkosc transmisji = 115200 
 	USART_InitStructure.USART_BaudRate = 115200; 
 
	// Init - USART 
	USART_Init(COM_USART, &USART_InitStructure);  
	USART_ClearFlag(COM_USART,USART_FLAG_TC); 
	USART_ITConfig(COM_USART, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(COM_USART, USART_IT_TXE, ENABLE); 
	// Wlaczenie USART 
	USART_Cmd(COM_USART, ENABLE); 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void TIM_Configuration(void)  
{ 
 
#ifdef PILOT 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	 
	/* TIM configuration for PWN (needed by LCD) */ 
 
	/* Time base configuration */ 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;          
	TIM_TimeBaseStructure.TIM_Period = 1439ul;	// PWM = 50kHz Hz 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
 
	/* PWM1 Mode configuration: Channel3 */ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 720ul;	// 1440 / 720 = 50% 
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);//wlaczenie buforowania 
   
	TIM_ARRPreloadConfig(TIM4, ENABLE);//wlaczenie buforowania  
  	 
	/* TIM3 enable counter */ 
	TIM_Cmd(TIM4, ENABLE);   
#endif 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void I2C_Configuration(void)  
{ 
   
#ifdef QUAD 
 
	I2C_InitTypeDef  I2C_InitStructure; 
		 
    I2C_DeInit(PRESSURE_I2C); 
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C; 
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2; 
    I2C_InitStructure.I2C_OwnAddress1 = 0x30; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable; 
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; 
    I2C_InitStructure.I2C_ClockSpeed = 100000; 
     
    I2C_Cmd(PRESSURE_I2C, ENABLE); 
    I2C_Init(PRESSURE_I2C, &I2C_InitStructure); 
 
    I2C_AcknowledgeConfig(PRESSURE_I2C, ENABLE); 
#endif 				  
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void SysTick_Configuration(void)  
{ 
#ifdef QUAD 
 
	unsigned long int Settings; 
	unsigned long int SysTick_CLKSource = SysTick_CLKSource_HCLK_Div8; 
	unsigned long int Ticks = 9000000ul; 
 
	assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource)); 
 
	//Kontrola, czy wartosc poczatkowa nie przekracza max 
	if (Ticks > SYSTICK_MAXCOUNT)  while (1); 
 
	// Ustaw wartosc poczatkowa licznika 
	SysTick->LOAD = (Ticks & SYSTICK_MAXCOUNT) - 1; 
 
	// Ustaw priorytet przerwania 
	NVIC_SetPriority(SysTick_IRQn, 2); 
 
	// Ustaw wartosc aktualna licznika 
	SysTick->VAL = 0; 
 
	// Ustaw znaczniki wlaczenia SysTick IRQ i samego licznika 
	Settings = (1<<SYSTICK_TICKINT) | (1<<SYSTICK_ENABLE); 
 
	//Wybierz znacznik ustawien zrodla sygnalu zegarowego 
	if (SysTick_CLKSource == SysTick_CLKSource_HCLK) { 
		Settings |= SysTick_CLKSource_HCLK;  
	} else { 
		Settings &= SysTick_CLKSource_HCLK_Div8; 
	}  
 
	// Zapisz ustawienia do rejestru sterujacego SysTick (i wlacz licznik) 
	SysTick->CTRL = Settings; 
#endif 
 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void Delay(__IO uint32_t nCount) 
{ 
	for(; nCount != 0; nCount--); 
} 
