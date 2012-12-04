
#include "rfm12_controller.h"

//rfm12
#include "rfm12.h"

//sensors
#include "18b20.h"
#include "gps.h"
#include "MPL115A2.h"

//others
#include <stdio.h>
#include <string.h>

/*! Zmienne globalne */
volatile uint8_t rf12_data_type;

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_init() {
	rf12_data_type = rf12_temperature;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send() {

	// check if rfm12 is ready
	if((rf12_rx || rf12_tx || rf12_new)) return;

/*	// reset counter
	if (rf12_data_type > (1 << rf12_current)) {
		rf12_data_type = rf12_temperature;
	}
  */
	// reset counter
	if (rf12_data_type > rf12_tension) {
		rf12_data_type = rf12_temperature;
	}

	// send temperature
	if (rf12_data_type & rf12_temperature) {
		rf12_controller_send_temperatures();
	}
	
	// send GPS
	else if (rf12_data_type & rf12_gps) {
		rf12_controller_send_gps();	
	}

	// send pressure
	else if (rf12_data_type & rf12_pressure) {
		rf12_controller_send_pressur();	
	}

	// send tension
	else if (rf12_data_type & rf12_tension) {
		rf12_controller_send_tension();
	}

	// send current
	else if (rf12_data_type & rf12_current) {
		rf12_controller_send_current();	
	}

	// move to next enum
	rf12_data_type = (rf12_data_type << 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_temperatures() {

	// set CMD value
	temp_measurements_string[0] = RFM12_TEMPERATURE_CMD;

	// send string
	rf12_txstart(temp_measurements_string, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_pressur() {
	// set CMD value
	mpl115a2_pressure_string[0] = RFM12_PRESSURE_CMD;

	// send string
	rf12_txstart(mpl115a2_pressure_string, 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_gps() {

	// set CMD value
	gps_string[0] = RFM12_GPS_CMD;

	// send string
	rf12_txstart(gps_string, 0);

/*
char* gps_time(void); // zwraca tablice 12 znakow (rok,miesiac,dzien,godzina,minuta,sekunda) np. 120501120311
char* gps_latitude(void); //zwraca szerokosc geograficzna, 10 znakow, podaje wartosc w ulamkach stopnia, ostatni znak - polkula
char* gps_longitude(void); //zwraca dlugosc geograficzna, 11 znakow, podaje wartosc w ulamkach stopnia, ostatni znak - polkula
char* gps_speed(void); // zwraca predkosc w m/s, 5 znkow, najmniej znaczace dwie cyfry sa wartociami dziesietnymi i setnymi predkosci
char* gps_direction(void); //zwraca kierunek przemieszczania w stopniach, 5 znaki, najmniej znaczace dwie cyfry sa wartociami dziesietnymi i setnymi predkosci
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_tension() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_current() {

}
