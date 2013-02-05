 								   
/* Includes ------------------------------------------------------------------*/ 
#include "stm32f10x.h" 
 
// Configurations 
#include "qdt_config.h" 
#include "core_cm3.h" 

// Common files
#include "rfm12_controller.h" 
#include "rfm12.h" 
 
// Own libraries 
//#ifdef PILOT
//#include "sd.h"
//#endif

#ifdef QUAD
#include "gps.h" 
#include "18B20.h" 
#include "MPL115A2.h" 
#include "lcd.h"
#endif

 
/* Private typedef -----------------------------------------------------------*/ 
/* Private define ------------------------------------------------------------*/ 
/* Private macro -------------------------------------------------------------*/ 
/* Private variables ---------------------------------------------------------*/ 
static vu32 TimingDelay = 0;

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
void ADC_Configuration(void); 
void DMA_Configuration(void); 
 
/* Private functions ---------------------------------------------------------*/ 
void Delay_ms(u32 nCount);
void TimingDelay_Decrement(void);

// keep ADC measures
unsigned short int buforADC[8] = {0};
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
int main(void)  
{ 

	volatile unsigned long int i;
//	unsigned long int napiecie, temperatura;
//	unsigned char Tekst[7] = {"0\0"};

	uint8_t ret = 0; 
	char test[120]; 
	unsigned char wartoscADC1VTekst[7] = "text\0";
 
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

	/* DMA Configuration */
//	DMA_Configuration(); 

	/* ADC Configuration */ 
//	ADC_Configuration();

	/* SysTick Configuration */ 
	SysTick_Configuration(); 
 
	/* Init rfm12 module */ 
	rf12_init();   

#ifdef PILOT

	/* Init SD card */
	sd_init();

	/* Init LCD */
    S65_init();

	/* RFM12 - rx start */
	rf12_rxstart(); 

#else
	/* Init GPS sensor */ 
	gps_init(); 
 
	/* Init send/recive rfm12 controller */ 
	rf12_controller_init(); 

#endif
	
	// odcekaj po konfuguracji rfm12 
//	Delay_ms(10); 
/*
	LS020_fill_screen(GREEN);

		LS020_message_centerXY(20,30,GREEN,BLACK,"Test wyswietlacza");
		LS020_put_char_maxXY(10, 60, RED, GREEN, 5, "T");
		LS020_put_char_maxXY(50, 60, WHITE, GREEN, 5, "E");
		LS020_put_char_maxXY(90, 60, YELLOW, GREEN, 5, "S");
		LS020_put_char_maxXY(130, 60, BLUE, GREEN, 5, "T");
*/
	while (1)  					   
	{		  
		// blink LED 
		GPIO_ResetBits(LEDS_PORT, LED_BIT_1); //LED8 ON 
    	GPIO_SetBits(LEDS_PORT, LED_BIT_2);   //LED9 OFF 
		Delay_ms(100); 
		GPIO_SetBits(LEDS_PORT, LED_BIT_1);   //LED9 ON 
    	GPIO_ResetBits(LEDS_PORT, LED_BIT_2); //LED8 OFF 
		Delay_ms(100); 

#ifdef QUAD

		/*
	    // Tu nalezy umiescic glowny kod programu
	    napiecie = buforADC[0] * 8059/10000;                                     //przelicz wartosc wyrazona jako calkowita, 12-bit na rzeczywista
 	  	sprintf((char *)Tekst, "%d,%03d V\0", napiecie / 1000, napiecie % 1000); //Dzielenie calkowite wyznacza wartosc w V,  dzielenie modulo - czeasc po przecinku
		printf("%s", Tekst);
   		temperatura = (1430 - buforADC[1] * 8059/10000)*10/43+25;                //przelicz wartosc wyrazona jako calkowita, 12-bit na rzeczywista, wartosci typowe wg. Datasheet, 5.3.18, str. 75.
 	  	sprintf((char *)Tekst, "%2d C\0", temperatura); 
		printf("%s \n", Tekst);
		*/
		
#endif 

#ifdef PILOT
 
		if( !(rf12_rx || rf12_tx || rf12_new) )  {
			rf12_rxstart(); 
		}
 
		if( rf12_new )	{ 
 
			ret = rf12_rxfinish(test);	// sprawdY czy odebrano kompletn1 ramke 
 
			if(ret > 0 && ret < 254) {	// brak bledów CRC - odebrana ramka 
				// printf(test);		// wyolij odebrane dane do terminala PC 

			parse_rfm12(test, ret);
			draw_screen_1();
//			test[80] = 0;				// przytnij dane do 16 znaków ASCII 

			// poprawnie odebrane dane
			GPIO_WriteBit(LEDS_PORT, LED_BIT_5,
				(BitAction)(1 - GPIO_ReadOutputDataBit(LEDS_PORT, LED_BIT_5)));
			} 
			else 
				 
			if(!ret) { // wyst1pi3 b31d CRC lub d3ugooci ramki 
				// blink - CRC error
				GPIO_WriteBit(LEDS_PORT, LED_BIT_6,
					(BitAction)(1 - GPIO_ReadOutputDataBit(LEDS_PORT, LED_BIT_6)));
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

	// GPIO for COM
/* 
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
*/

	// GPIO for LEDs

	//LEDs 
	GPIO_InitStructure.GPIO_Pin = LED_BIT_1 | LED_BIT_2 | LED_BIT_3 | LED_BIT_4 | LED_BIT_5 | LED_BIT_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(LEDS_PORT, &GPIO_InitStructure);  

	// GPIO for rfm12
 
	// SPI - CS 
	GPIO_InitStructure.GPIO_Pin = RFM12_BIT_SS; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(RFM12_PORT_SS, &GPIO_InitStructure); 
 
	// SPI - INT 
	GPIO_InitStructure.GPIO_Pin = RFM12_BIT_INT; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(RFM12_PORT_INT, &GPIO_InitStructure); 
 
	// SPI - SCK, MOSI 
	GPIO_InitStructure.GPIO_Pin = RFM12_BIT_SCK | RFM12_BIT_MOSI | RFM12_BIT_MISO; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(RFM12_PORT_SPI, &GPIO_InitStructure); 

#ifdef PILOT 
	// GPIO for LCD
 
	//PWM 
	GPIO_InitStructure.GPIO_Pin = PWM_BIT_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(PWM_PORT, &GPIO_InitStructure); 

	// GPIO for SD
 
    // SD - CS 
    GPIO_InitStructure.GPIO_Pin = SD_BIT_SS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SD_PORT_SS, &GPIO_InitStructure);

    //SD - SCK, MISO, MOSI
    GPIO_InitStructure.GPIO_Pin = SD_BIT_SCK | SD_BIT_MISO | SD_BIT_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SD_PORT_SPI, &GPIO_InitStructure);

	//SD - DETECT
    GPIO_InitStructure.GPIO_Pin = SD_BIT_DETECT;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SD_PORT_DETECT, &GPIO_InitStructure);


	// GPIO for LCD
 
    // SD - CS 
    GPIO_InitStructure.GPIO_Pin = LCD_BIT_SS;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_PORT_SS, &GPIO_InitStructure);

    //SD - SCK, MOSI
    GPIO_InitStructure.GPIO_Pin = LCD_BIT_SCK | LCD_BIT_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT_SPI, &GPIO_InitStructure);

/*
    // poprawienie bledu RUDEGO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //SD - SCK, MOSI
    GPIO_InitStructure.GPIO_Pin = LCD_BIT_SCK | LCD_BIT_MOSI;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PORT_SPI, &GPIO_InitStructure);
*/
	// RS & RESET
    GPIO_InitStructure.GPIO_Pin   = LCD_PIN_RESET | LCD_PIN_RS;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_PORT_LS020, &GPIO_InitStructure);
#endif 

#ifdef QUAD 

	// GPIO for I2C

    GPIO_InitStructure.GPIO_Pin =  PRESSURE_BIT_1 | PRESSURE_BIT_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
    GPIO_Init(PRESSURE_PORT, &GPIO_InitStructure); 
 
	// GPIO for GPS
 
	// USART2 - TX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	GPIO_InitStructure.GPIO_Pin = GPS_BIT_TX; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPS_PORT, &GPIO_InitStructure); 
 
	// USART2 - RX 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = GPS_BIT_RX; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPS_PORT, &GPIO_InitStructure); 

	// GPIO for TEMPERATURE
 
	// TEMPERATURE 
	GPIO_InitStructure.GPIO_Pin = TEMPERATURE_BIT_1 | TEMPERATURE_BIT_2 | TEMPERATURE_BIT_3 | TEMPERATURE_BIT_4 |TEMPERATURE_BIT_5 | TEMPERATURE_BIT_6;	          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(TEMPERATURE_PORT, &GPIO_InitStructure);	 
  
/*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;   
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                                   //wejscie analogowe
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
						*/
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
 
    	/* PCLK1 = HCLK/2 */ 
		RCC_PCLK1Config(RCC_HCLK_Div2); 
 
//    	/* PLLCLK = 8MHz * 9 = 72 MHz */ 
//		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 

		/* PLLCLK = HSE*7 czyli 8MHz * 7 = 56 MHz */  
  		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
 
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
 
#ifdef QUAD 
	/* GPIO A/B/C/D clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); 
#else
	/* GPIO A/B/C/D/E/G clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOG, ENABLE); 
#endif
  
	/* AFIO clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
 
	/* USART1 clock enable */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  

	/* TIM1 clock enable */	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); 
 
#ifdef QUAD 
	/* USART2 clock enable */ 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  
  
	/* I2C clock enable */	 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 

	/* ADC1 clock enable */	 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* DMA1 clock enable */	 
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	// ADCCLK = PCLK2/4 = 56 MHz /4 = 14 MHz (maksymalna mozliwa)
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);  

#endif 

#ifdef PILOT 
	/* TIM3 clock enable */	 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	/* SPI2 clock enable */ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); 

	/* SPI3 clock enable */ 
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); 
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
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 	 	 
	//Priorytet grupowy 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	//Podpriorytet 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	//Wlaczenie obslugi IRQ 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
 

	/* NVIC for TIM1 */ 

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

  	//przerwanie UP (przepelnienie) timera1
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
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
	/* EXTI for rfm12 */ 
	EXTI_InitTypeDef EXTI_InitStructure; 

#ifdef QUAD 
	//Ustawienia zrodla przerwania 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource9); 

	//Wybor lini 
	EXTI_InitStructure.EXTI_Line = EXTI_Line9; 
#else
	//Ustawienia zrodla przerwania 
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource8); 

	//Wybor lini 
	EXTI_InitStructure.EXTI_Line = EXTI_Line8; 
#endif
	 
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
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(RFM12_SPI, &SPI_InitStructure); 
	SPI_Cmd(RFM12_SPI, ENABLE);  

#ifdef PILOT
	
	/* SPI for SD */ 

	// Konfiguracja SPI
	SPI_InitStructure.SPI_Direction =  SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;              
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;  
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SD_SPI, &SPI_InitStructure);   
    SPI_Cmd(SD_SPI, ENABLE);


	/* SPI for LCD */ 

	// Konfiguracja SPI
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial= 7;
    SPI_Init(LCD_SPI, &SPI_InitStructure);
    SPI_Cmd(LCD_SPI, ENABLE);

#endif
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
 
 
	/* USART for GPS */ 
 
#ifdef QUAD 
	// reset 
	USART_DeInit(GPS_USART); 
 	// predkosc transmisji = 9600 
 	USART_InitStructure.USART_BaudRate = 9600; 
	//Tryb pracy (Rx - odbior, Tx - nadawanie) 
	USART_InitStructure.USART_Mode = USART_Mode_Rx; 
 
	// Init - USART 
	USART_Init(GPS_USART, &USART_InitStructure); 
	USART_ClearFlag(GPS_USART, USART_FLAG_TC); 
	// Wlaczenie przerwan wywolywanych przez USART2 
	USART_ITConfig(GPS_USART, USART_IT_RXNE, ENABLE); 
	// Wlaczenie USART 
	USART_Cmd(GPS_USART, ENABLE); 
#endif 
 
	/* USART for COM */ 
/* 
	// reset 
	USART_DeInit(COM_USART); 
 	// predkosc transmisji = 115200 
 	USART_InitStructure.USART_BaudRate = 115200; 
	//Tryb pracy (Rx - odbior, Tx - nadawanie) 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
 
	// Init - USART 
	USART_Init(COM_USART, &USART_InitStructure);  
	USART_ClearFlag(COM_USART,USART_FLAG_TC); 
	USART_ITConfig(COM_USART, USART_IT_RXNE, ENABLE); 
	USART_ITConfig(COM_USART, USART_IT_TXE, ENABLE); 
	// Wlaczenie USART 
	USART_Cmd(COM_USART, ENABLE); 
*/

} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void TIM_Configuration(void)  
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

#ifdef PILOT 
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
#endif
 
	/* TIM1 configuration */ 
 
	/* Time base configuration */ 
	TIM_TimeBaseStructure.TIM_Prescaler = 56000-1;          

#ifdef PILOT	
	TIM_TimeBaseStructure.TIM_Period = 10;		// 10ms
#else
	TIM_TimeBaseStructure.TIM_Period = 1000;	// 1sek.
#endif

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);	

	/* TIM1 enable counter */ 
	TIM_Cmd(TIM1, ENABLE);   

#ifdef PILOT 

	/* TIM configuration for PWM (needed by LCD) */ 
 
	/* Time base configuration */ 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;          
	TIM_TimeBaseStructure.TIM_Period = 1439ul;	// PWM = 50kHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
 
	TIM_TimeBaseInit(PWM_TIMER, &TIM_TimeBaseStructure); 


	/* PWM1 Mode configuration: Channel3 */ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 720ul;	// 1440 / 720 = 50% 
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High; 
	TIM_OC3Init(PWM_TIMER, &TIM_OCInitStructure); 
 
	TIM_OC3PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);//wlaczenie buforowania 
   
	TIM_ARRPreloadConfig(PWM_TIMER, ENABLE);//wlaczenie buforowania  
  	 
	/* TIM3 enable counter */ 
	TIM_Cmd(PWM_TIMER, ENABLE);   
#endif 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void I2C_Configuration(void)  
{ 
   
#ifdef QUAD 
 
	I2C_InitTypeDef I2C_InitStructure; 
		 
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
void DMA_Configuration(void) {
#ifdef QUAD 
	//konfigurowanie DMA
/*
	#define ADC1_DR_Address 0x4001244C;                                             //adres rejestru ADC1->DR
  
	DMA_InitTypeDef DMA_InitStructure;
  
	DMA_DeInit(DMA1_Channel1);                                                      //Usun ewentualna poprzednia konfiguracje DMA
  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned long int)ADC1_DR_Address;  //Adres docelowy transferu
	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned long int)&buforADC;            //Adres poczatku bloku do przeslania
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                              //Kierunek transferu
	DMA_InitStructure.DMA_BufferSize = 2;                                           //Liczba elementow do przeslania (dlugosc bufora)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                //Wylaczenie automatycznego zwiekszania adresu po stronie ADC
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                         //Wlaczenie automatycznego zwiekszania adresu po stronie pamieci (bufora)
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;     //Rozmiar pojedynczych przesylanych danych po stronie ADC (HalfWord = 16bit)
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;             //Rozmiar pojedynczych przesylanych danych po stronie pamieci (bufora)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                                 //Tryb dzialania kontrolera DMA - powtarzanie cykliczne
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                             //Priorytet DMA - wysoki
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                    //Wylaczenie obslugi transferu z pamieci do pamieci
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);                                    //Zapis konfiguracji

	//Wlacz DMA, kanal 1
	DMA_Cmd(DMA1_Channel1, ENABLE);  
	*/
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////// 
void ADC_Configuration(void) {
#ifdef QUAD 
/*
	//konfigurowanie przetwornika AC
	ADC_InitTypeDef ADC_InitStructure;
	  
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                 			 		//Jeden przetwornik, praca niezalezna
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        				//Pomiar dwoch kanalow, konieczne skanowanie kanalow 
  	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                					//Pomiar w trybie ciaglym
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//Brak wyzwalania zewnetrznego
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;            					//Wyrownanie danych do prawej - 12 mlodszych bitow znaczacych
	ADC_InitStructure.ADC_NbrOfChannel = 8; 	                          				//Liczba uzywanych kanalow =2
	ADC_Init(ADC1, &ADC_InitStructure);                                 				//Incjalizacja przetwornika

	// Voltage - Battery 1 
	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_1_CELL_1, 1, ADC_SampleTime_1Cycles5); 
  	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_1_CELL_2, 2, ADC_SampleTime_1Cycles5); 
	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_1_CELL_3, 1, ADC_SampleTime_1Cycles5); 

	// Voltage - Battery 2 
  	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_2_CELL_1, 2, ADC_SampleTime_1Cycles5); 
	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_2_CELL_2, 1, ADC_SampleTime_1Cycles5); 
  	ADC_RegularChannelConfig(ADC1, ADC_VOLTAGE_2_CELL_3, 2, ADC_SampleTime_1Cycles5); 
	
	// Battery current (battery 1&2)
	ADC_RegularChannelConfig(ADC1, ADC_CURRENT_1, 1, ADC_SampleTime_1Cycles5); 
  	ADC_RegularChannelConfig(ADC1, ADC_CURRENT_2, 2, ADC_SampleTime_1Cycles5); 

  	ADC_DMACmd(ADC1,ENABLE);                                            				//Wlaczenie DMA dla ADC1
  	ADC_Cmd(ADC1, ENABLE);						         	            				//Wlacz ADC1

	ADC_ResetCalibration(ADC1);	                                        				//Reset rejestrow kalibracyjnych ADC1
	while(ADC_GetResetCalibrationStatus(ADC1));	                        				//Odczekanie na wykonanie resetu

	ADC_StartCalibration(ADC1);	                                        				//Kalibracja ADC1
	while(ADC_GetCalibrationStatus(ADC1));    	                        				//Odczekanie na zakonczenie kalibracji ADC1

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);                             				//rozpocznij przetwarzanie AC
*/
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////// 
void SysTick_Configuration(void)  
{ 

	unsigned long int Settings; 
	unsigned long int SysTick_CLKSource = SysTick_CLKSource_HCLK; 
	unsigned long int Ticks = 72000ul;  // 1ms 
 
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
 
} 
 
//////////////////////////////////////////////////////////////////////////////////////////////////// 
void Delay_ms(u32 nCount)
{
  TimingDelay = nCount;

  /* Enable the SysTick Counter */
  SysTick->CTRL |= ((u32)0x00000001);
  
  while(TimingDelay != 0)
  {
  }

  /* Disable the SysTick Counter */
  SysTick->CTRL |= ((u32)0xFFFFFFFE);

  /* Clear the SysTick Counter */
  SysTick->VAL |= ((u32)0x00000000);
}

//////////////////////////////////////////////////////////////////////////////////////////////////// 
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}
