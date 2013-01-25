#ifndef _GPS_H
#define _GPS_H

#include <stm32f10x.h>

#include <stdio.h>
#include <string.h>
/*
	W metodzie USART2_IRQHandler (plik stm32f10x_it.c) znajduje sie obsluga przerwania 
	USART2 w ktorej odbywa sie zapisaywanie danych do buforow. Przejmowane zdania to VTG oraz RMC.

	Podlaczenie anteny:
	1: VCC
	2: GND
	3: RX (narazie nie wspierany)
	4: TX
	6: GND
*/

// keep measured GPS valuesas array of chars
extern volatile char gps_string[50];

// ustawia GPIO, USART2, NVIC, wywalsc raz w main.c
void gps_init(void);
						
char* gps_time(void); // zwraca tablice 12 znakow (rok,miesiac,dzien,godzina,minuta,sekunda) np. 120501120311
char* gps_latitude(void); //zwraca szerokosc geograficzna, 10 znakow, podaje wartosc w ulamkach stopnia, ostatni znak - polkula
char* gps_longitude(void); //zwraca dlugosc geograficzna, 11 znakow, podaje wartosc w ulamkach stopnia, ostatni znak - polkula
char* gps_speed(void); // zwraca predkosc w m/s, 5 znkow, najmniej znaczace dwie cyfry sa wartociami dziesietnymi i setnymi predkosci
char* gps_direction(void); //zwraca kierunek przemieszczania w stopniach, 5 znaki, najmniej znaczace dwie cyfry sa wartociami dziesietnymi i setnymi predkosci

void gps_pool(void); // obsluga przerwania USART

#endif /*_GPS_H*/
