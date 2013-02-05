#ifndef _RFM12_CONTROLLER_H
#define _RFM12_CONTROLLER_H

//pin mappings
#include "qdt_config.h" 

/***
 * Communicatin commands
 */

#define RFM12_TEMPERATURE_CMD	'A'
#define RFM12_GPS_CMD			'B'
#define RFM12_PRESSURE_CMD		'C'
#define RFM12_TENSION_CMD		'D'
#define RFM12_CURRENT_CMD		'E'


//! typ wyliczeniowy opisujacy stan rfm12
enum rf12_data_types { 
	rf12_temperature	= 1 << 0, 
	rf12_gps			= 1 << 2, 
	rf12_pressure		= 1 << 3, 
	rf12_tension		= 1 << 4, 
	rf12_current		= 1 << 5 
};

//! aktualny stan rfm12
extern volatile uint8_t rf12_data_type; 

//! clear data
void rf12_controller_init(void);

#ifdef QUAD

//! send next data
void rf12_controller_send(void);

//! send values from sensors
void rf12_controller_send_temperatures(void);
void rf12_controller_send_pressur(void);
void rf12_controller_send_gps(void);
void rf12_controller_send_tension(void);
void rf12_controller_send_current(void);

#else

void parse_rfm12 (char *data, int len);

#endif

#endif 
