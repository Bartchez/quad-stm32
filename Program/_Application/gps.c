#include "gps.h"

volatile char gps_string[50];

/* Private vars */
volatile uint16_t RxCounter;
volatile char RxBuffer[256]; //bufor tymczasowy, przechowuje dane odebrane z USART2
volatile char RxBufferVTG[256]; //przechowuje wartosci przepusane z RxBuffer, jezeli zdanie rozpoczyna sie na VTG
volatile char RxBufferRMC[256]; //przechowuje wartosci przepusane z RxBuffer, jezeli zdanie rozpoczyna sie na RMC

////////////////////////////////////////////////////////////////////////////////////////////////////
void gps_init(void)
{
  //temp
  int i;

  //wyczyszczenie buforow
  for(i=0; i<256; i++) {
    RxBuffer[i] = '0';
	RxBufferRMC[i] = '0';
	RxBufferVTG[i] = '0';
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
char* gps_time()
{
	static char array[12];

	//rok
	array[0] = RxBufferRMC[61];
	array[1] = RxBufferRMC[62];
	
	//miesiac
	array[2] = RxBufferRMC[59];
	array[3] = RxBufferRMC[60];
	
	//dzien
	array[4] = RxBufferRMC[57];
	array[5] = RxBufferRMC[58];
	
	//godziny
	array[6] = RxBufferRMC[7];
	array[7] = RxBufferRMC[8];
	
	//minuty
	array[8] = RxBufferRMC[9];
	array[9] = RxBufferRMC[10];
	
	//sekundy
	array[10] = RxBufferRMC[11];
	array[11] = RxBufferRMC[12];
	
	return array;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
char* gps_latitude(void) 
{
	uint8_t i;
   	static char array[10];

	//aktualizacja szerokosci geograficznej
	array[0] = RxBufferRMC[20];
	array[1] = RxBufferRMC[21];

	//przeliczenie wartosci minut na wartosc ulamkowa stopnia
	i  = ((RxBufferRMC[22] - 0x30) * 100000);
	i += ((RxBufferRMC[23] - 0x30) * 10000);
	i += ((RxBufferRMC[25] - 0x30) * 1000);
	i += ((RxBufferRMC[26] - 0x30) * 100);
	i += ((RxBufferRMC[27] - 0x30) * 10);
	i += ((RxBufferRMC[28] - 0x30) * 1);
	i  = i / 60;

	//aktualizacja szerokosci geograficznej w ciagu
	array[3] = i / 1000 + 0x30;
	i = i % 1000;
	array[4] = i / 100 + 0x30;
	i = i % 100;
	array[5] = i / 10 + 0x30;
	i = i % 10;
	array[6] = i % 10 + 0x30;
	array[7] = '0';
	array[8] = '0';
	array[9] = RxBufferRMC[30];

	return array;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
char* gps_longitude(void) {
	uint8_t i;
   	static char array[11];

	//aktualizacja dlugosci geograficznej
	array[0] = RxBufferRMC[32];
	array[1] = RxBufferRMC[33];
	array[2] = RxBufferRMC[34];

	//przeliczenie wartosci minut na wartosc ulamkowa stopnia
	i  = ((RxBufferRMC[35] - 0x30) * 100000);
	i += ((RxBufferRMC[36] - 0x30) * 10000);
	i += ((RxBufferRMC[38] - 0x30) * 1000);
	i += ((RxBufferRMC[39] - 0x30) * 100);
	i += ((RxBufferRMC[40] - 0x30) * 10);
	i += ((RxBufferRMC[41] - 0x30) * 1);
	i  = i / 60;

	//aktualizacja dlugosci geograficznej w ciagu
	array[4] = i / 1000 + 0x30;
	i = i % 1000;
	array[5] = i / 100 + 0x30;
	i = i % 100;
	array[6] = i / 10 + 0x30;
	i = i % 10;
	array[7] = i % 10 + 0x30;
	array[8] = '0';
	array[9] = '0';
	array[10] = RxBufferRMC[43];

	return array;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
char* gps_speed(void) {
	
	uint8_t i;
   	static char array[5];

	//przeliczenie wartosci km/h na m/s
	//najmniej znaczace dwie cyfry sa wartociami
	//dziesietnymi i setnymi predkosci
	
	i  = (RxBufferVTG[26] - 0x30) * 100000;
	i += (RxBufferVTG[27] - 0x30) * 10000;
	i += (RxBufferVTG[28] - 0x30) * 1000;
	i += (RxBufferVTG[30] - 0x30) * 100;
	i  = i / 36;

	i = i % 100000;
	array[0] = (i / 10000) + 0x30;
	i = i % 10000;
	array[1] = (i / 1000) + 0x30;
	i = i % 1000;		 
	array[2] = (i / 100) + 0x30;
	i = i % 100;
	array[3] = (i / 10) + 0x30;
	array[4] = (i % 10) + 0x30;
	
	return array;				
}

////////////////////////////////////////////////////////////////////////////////////////////////////
char* gps_direction(void) {

   	static char array[5];

	array[0] = RxBufferRMC[50];
	array[1] = RxBufferRMC[51];
	array[2] = RxBufferRMC[52];
	array[3] = RxBufferRMC[54];
	array[4] = RxBufferRMC[55];
	
	return array;				
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void gps_pool(void) {
	int i = 0;

	//zapis odczytanego znaku
	RxBuffer[RxCounter] = (USART_ReceiveData(USART2) & 0x7F);
	//inkrementacja licznika danych
	RxCounter++;
	//zabezpieczenie przed wprowadzeniem zbyt dlugiego ciagiu znakow
	if(
		(RxBuffer[RxCounter - 2] != '\r') &&
		(RxBuffer[RxCounter - 1] != '\n') &&
		(RxCounter == 255)
	)
		{RxCounter = 253;}
	//zakonczenie transmisji ciagu znakiem nowej linii
	else if(
		(RxBuffer[RxCounter - 2] == '\r') &&
		(RxBuffer[RxCounter - 1] == '\n')
	)
	{

		//odebrany ciag jest komunikatem VTG
		if(
			(RxBuffer[0] == '$') &&
			(RxBuffer[1] == 'G') &&
			(RxBuffer[2] == 'P') &&
			(RxBuffer[3] == 'V') &&
			(RxBuffer[4] == 'T') &&
			(RxBuffer[5] == 'G')
		)
		{
			//skopiowanie odebranego ciagu do RxBufferVTG
			//oraz wyzerowanie RxBuffer
			for(i=0; i<256; i++)
			{
				RxBufferVTG[i] = RxBuffer[i];
				// printf("%c", RxBuffer[i]);
				RxBuffer[i] = 0;
			}
			//wyzerowanie licznika
			RxCounter = 0;
		}
		//odebrany ciag jest komunikatem RMC
		else if(
			(RxBuffer[0] == '$') &&
			(RxBuffer[1] == 'G') &&
			(RxBuffer[2] == 'P') &&
			(RxBuffer[3] == 'R') &&
			(RxBuffer[4] == 'M') &&
			(RxBuffer[5] == 'C')
		)
		{
			//skopiowanie odebranego ciagu do RxBufferRMC
			//oraz wyzerowanie RxBuffer
			for(i=0; i<256; i++)
			{
				RxBufferRMC[i] = RxBuffer[i];
				// printf("%c", RxBuffer[i]);
				RxBuffer[i] = 0;
			}
			//wyzerowanie licznika
			RxCounter = 0;
		}
		//odebrany ciag nie nalezy do oczekiwanych komunikatów
		else
		{
			//wyczyszczenie bufora
			for(i=0; i<256; i++)
				{RxBuffer[i] = 0;}
			//wyzerowanie licznika
			RxCounter = 0;
		}
	}

	// create output string
    sprintf(gps_string, "X:%s,%s,%s,%s,%s", gps_time(), gps_latitude(),
					  									  gps_longitude(), gps_speed(), 
														  gps_direction());    

}

