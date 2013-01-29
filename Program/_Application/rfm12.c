
//rfm12
#include "rfm12.h"

//stm32
#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"

//others
#include <stdio.h>
#include <string.h>

/*! Zmienne globalne */
volatile uint8_t rf12_rx;
volatile uint8_t rf12_tx;
volatile uint8_t rf12_new;
volatile uint8_t rf12_index = 0;
uint8_t rf12_data[rf12_max_length + 10];	// +10 nadmiarowo na pozosta3e czeoci ramki 

//default preamble (altrernating 1s and 0s)
#define PREAMBLE 0xAA

//default synchronization pattern
#define SYNC_MSB 0x2D
#define SYNC_LSB 0xD4

//hardware spi helper macros
#define SS_ASSERT()		GPIO_ResetBits(RFM12_PORT_SS, RFM12_BIT_SS)
#define SS_RELEASE()	GPIO_SetBits(RFM12_PORT_SS, RFM12_BIT_SS)

//#pragma mark - RFM12 Configuration
void rfm12_delay(__IO uint32_t nCount);

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_init( void ) {
	// wyzerowanie flag
	rf12_rx = 0; rf12_tx = 0; rf12_new = 0;

	// reset rejestrow na rfm12
//	rf12_trans(0xCA82);
//	rf12_trans(0xFE00);
//	rfm12_delay(0x00ffff);
//	rf12_trans(0x0000);

	// ustawinie nowych wartosci w rejestrach
	rf12_trans(0x80D8);//enable register,433MHz,12.5pF
	rf12_trans(0x82D8);//enable receive,!PA
	rf12_trans(0xA640);//434MHz
	rf12_trans(0xC647);//
	rf12_trans(0x94A0);//VDI,FAST,134kHz,0dBm,-103dBm
	rf12_trans(0xC2AC);
	rf12_trans(0XCC77);//
	rf12_trans(0xCA80);
	rf12_trans(0xCA83);//FIFO8,SYNC,
	rf12_trans(0xC49B);
	rf12_trans(0x9850);//!mp,9810=30kHz,MAX OUT
	rf12_trans(0xE000);//NOT USE
	rf12_trans(0xC800);//NOT USE
	rf12_trans(0xC000);//1.0MHz,2.2V
	rf12_trans(0x0000);

	// odczekaj po inicializacji
	rfm12_delay(0x00ffff);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t crc_update(uint16_t crc, uint8_t serialData) {
	uint16_t tmp;
	uint8_t j;

	tmp = serialData << 8;
    for (j=0; j<8; j++)	{
            
		if ((crc^tmp) & 0x8000) {
			crc = (crc<<1) ^ 0x1021;	
        } else {
			crc = crc << 1;
		}	
        tmp = tmp << 1;
    }
	return crc;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_pool() { 
	uint8_t data = 0; 
	if (rf12_rx) { 
		if (rf12_index < rf12_max_length) { 
			data = rf12_read_fifo() & 0x00FF; 
			rf12_data[rf12_index++] = data; 
		} else { 
			rf12_trans(0x8208); 
			rf12_rx = 0; 
			rf12_new = 1; 
			return; 				// na pewno bedzie b3edna ramka 
		} 
		if (rf12_index >= rf12_data[0] + 3) { 
 
			rf12_trans(0x8208); 
			rf12_rx = 0; 
			rf12_new = 1;	// poprawnie zakonczona ramka 
		} 
	} 
	
	else if (rf12_tx) { 
		rf12_trans(0xB800 | rf12_data[rf12_index]); 
 
		if (!rf12_index)	{ 
			rf12_tx = 0; 
			printf("tx off"); 
			rf12_trans(0x8208);		// TX off 
			rf12_read_status(); 
		} else { 
			rf12_index--; 
		} 
		rf12_read_status(); 
	} 
} 
 
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t rf12_rxstart(void) { 
	if (rf12_new) 
		return(1);			//bufor jeszcze nie pusty 
	if (rf12_tx) 
		return(2);			//trwa nadawanie 
	if (rf12_rx) 
		return(3);			//trwa odbieranie 
 
	rf12_index = 0; 
	rf12_rx = 1; 

	rf12_trans(0x8058); 
 
	// fifo reset 
	rf12_trans(0xCA81);		// disable FIFO mode 
	rf12_trans(0xCA83);		// enable FIFO mode 
 
	rf12_trans(0x82D8);		// RX on 
 
	return(0);				// wszystko w porzadku 
} 
 
/*! Funkcja sprawdzaj1ca i finalizuj1ca odbiór kompletnej ramki danych 
 * 
 *  *data - bufor na odebran1 ramke 
 * 
 *  ret = ilooa bajtów ramki danych 
 *  jeoli BLYD i: 
 *  ret = 255 to oznacza, ?e jeszcze trwa odbiór ramki 
 *  ret = 254 to oznacza, ?e jeszcze nie odczytaliomy poprzedniej ramki 
 * 
 * */ 
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t rf12_rxfinish( char *data ) { 
	uint16_t crc, crc_chk = 0; 
	uint8_t i, size = rf12_data[0]; 
	if (rf12_rx) return(255);		//odbiór jeszcze nie zakonczony 
	if (!rf12_new) return(254);		//jeszcze stare dane w buforze 
 
	if (size > rf12_max_length ) { 
		data[0] = 0; 
		rf12_new = 0; 
		return 0; // b31d wielkooci ramki 
	} 
 
	for (i=0; i<size +1 ; i++) {
		crc_chk = crc_update(crc_chk, rf12_data[i]); 
	}
	
	crc = rf12_data[i++]; 
	crc |= rf12_data[i] << 8; 
	rf12_new = 0; 

	if (crc != crc_chk) {
		return(0);		//b31d sumy CRC lub rozmiaru ramki 
	}
	
	else { 
		for(i=0; i<size; i++) {
			data[i] = rf12_data[i+1]; 
		}
		
		data[size] = 0;		// zakonczenie ramki zerem 
		return(size);		// rozmiar odebranej ramki w bajtach 
	} 
} 
 
/*! Funkcja inicjalizuj1ca transmisje pakietu danych 
 *  *data - wskaYnik na bufor danych 
 *  size = 0 gdy transmitujemy C-String (3ancuch tekstowy ASCII) 
 *  size > 0 gdy transmitujemy dane binarne 
 * 
 *  ret = 0 - nadawanie zainicjalizowane poprawnie 
 *  BLEDY 
 *  ret = 1 - trwa nadawanie poprzedniej ramki 
 *  ret = 2 - trwa odbieranie ramki 
 *  ret = 3 - zbyt du?a ramka danych 
 * 
 */ 
////////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t rf12_txstart( char *data, uint8_t size ) { 
	uint8_t i, l; 
	uint16_t crc; 
 
	if (!size) {
		size = strlen(data); 
	}
	
	if (rf12_tx) 
		return(1);			//nadawanie w toku 
	if (rf12_rx) 
		return(2);			//odbieranie w toku 
	if (size > rf12_max_length) 
		return(3);			//za d3uga ramka danych 
 
	rf12_tx = 1; 
	//zwiekszenie ramki o 10 na potrzeby: 
	// preambu3y (3bajty), 2 bajty synchro, 1 bajt rozmiar ramki, 
	// sumy CRC16 (2 bajty) oraz na koncu dwa bajty 0xAA 
	rf12_index = size + 9;			//act -10 
 
	i = rf12_index; 
	rf12_data[i--] = PREAMBLE; 
	rf12_data[i--] = PREAMBLE;
	rf12_data[i--] = PREAMBLE; 
	rf12_data[i--] = SYNC_MSB; 
	rf12_data[i--] = SYNC_LSB; 
	rf12_data[i--] = size; 
	crc = crc_update(0, size); 
	for(l=0; l<size; l++) { 
		rf12_data[i--] = data[l]; 
		crc = crc_update(crc, data[l]); 
	} 
	rf12_data[i--] = (crc & 0x00FF); 
	rf12_data[i--] = (crc >> 8); 
	rf12_data[i--] = PREAMBLE; 
	rf12_data[i--] = PREAMBLE; 
 
	rf12_trans(0x8228); //OPEN PA 
	rfm12_delay(0x00ffff); 
	rf12_trans(0x8238); // TX ON 
 
	rf12_read_status(); 
 
	return 0;			//wszystko OK 
} 
 
////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_allstop(void) { 
	rf12_rx = 0; 
	rf12_tx = 0; 
	rf12_new = 0; 
	rf12_trans(0x8208);		//stan bezczynnooci rfm12 
	rf12_read_status();		//pusty odczyt 
} 

////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t rf12_send(uint16_t data) {

	uint16_t ret = 0;

	SS_ASSERT();

	// Wyslanie bajtu z SD
	while (SPI_I2S_GetFlagStatus(RFM12_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(RFM12_SPI, data);

	// Odebranie bajtu
	while (SPI_I2S_GetFlagStatus(RFM12_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	ret = SPI_I2S_ReceiveData(RFM12_SPI);

	while (SPI_I2S_GetFlagStatus(RFM12_SPI, SPI_I2S_FLAG_BSY) == SET);

	SS_RELEASE();

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_trans(uint16_t data) {
	rf12_send(data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t rf12_read_status() {
	return rf12_send(0x0000);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
uint16_t rf12_read_fifo(void) {
	return rf12_send(0xB000);	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rfm12_delay(__IO uint32_t nCount)
{
    for(; nCount != 0; nCount--);
}
