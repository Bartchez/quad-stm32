#ifndef _RFM12_H
#define _RFM12_H

//pin mappings
#include "qdt_config.h" 

//rfm12 hardware 
#include "rfm12_hw.h" 

/* RF12 wielkooa bufora - istotne jeoli korzystamy z przerwan */ 
#define rf12_max_length	100		//max length 243 

/*
//! typ wyliczeniowy opisujacy stan rfm12
enum rf12_modes { 
	rf12_idle	= 0, 
	rf12_rx		= 1 << 0, 
	rf12_tx		= 1 << 1, 
	rf12_new	= 1 << 2 
};

//! aktualny stan rfm12
extern volatile uint8_t rf12_mode; 
*/

extern volatile uint8_t rf12_rx; 
extern volatile uint8_t rf12_tx; 
extern volatile uint8_t rf12_new; 
 
//! transmisja 16 bitow do rfm12 
void rf12_trans(uint16_t wert); 
//! wyslanie 0x0000 do rmf12 i odczytanie rejestru STATUS 
uint16_t rf12_read_status(void); 
//! wyslanie 0xB000 i odczytanie rejestru 
uint16_t rf12_read_fifo(void); 
 
//! inicjalizacja rfm12 
void rf12_init(void); 
 
//! obsluga przerwania 
void rf12_pool(void); 
 
//! start odbioru ramki danych 
uint8_t rf12_rxstart(void); 
 
//! odczytanie do bufora gdy zostanie poprawnie odebrana 
uint8_t rf12_rxfinish(char *data); 
 
//! start transmisji/nadawania ramki z bufora o wielkooci size 
uint8_t rf12_txstart(char *data, uint8_t size); 
  
//! zatrzymanie wszystkich operacji nadwania i odbioru 
void rf12_allstop(void); 
 
#endif 
