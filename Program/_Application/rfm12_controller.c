
#include "rfm12_controller.h"

//rfm12
#include "rfm12.h"

//sensors
#include "18b20.h"
#include "gps.h"
#include "MPL115A2.h"

//others
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*! Zmienne globalne */
volatile uint8_t rf12_data_type;

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_init() {
	rf12_data_type = rf12_temperature;
}

#ifdef QUAD

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
		// read temp values from 6 sensors and save to temp_measurements array 
		ds18b20_read_temps();

		rf12_controller_send_temperatures();
	}
	
	// send GPS
	else if (rf12_data_type & rf12_gps) {
		rf12_controller_send_gps();	
	}

	// send pressure
	else if (rf12_data_type & rf12_pressure) {
		// read pressure values sensor and save to mpl115a2_pressure ivar 
		mpl115a2_read_pressure(); 

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
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_tension() {

}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rf12_controller_send_current() {

}

#else

char buff[30];

/*C substring function: It returns a pointer to the substring */
////////////////////////////////////////////////////////////////////////////////////////////////////
void substring(char *string, int position, int length) {
    
    int c;
    
    for (c = 0 ; c < length ; c++) {
		buff[c] = string[position + c -1];
    }
    
    buff[length] = '\0';
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void rewrite(char* begin, uint8_t count) {
	uint8_t i;
	
	for (i=0; i<count; i++) {
		begin[i] = buff[i];
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void parse_rfm12(char *data, int len) {
	int c = 0, i, pos = 1;
    
	// get message id
	char id = data[0];

	// exit if empty array
	if (len == 0) return;
        
	// iterate all array and find ',' sign
	for (i = 1; i <= len; i++) {

        // if comma or end of message
        if (data[i] == ',' || i == len) {

            // get substring
            substring(data, pos + 1, i - pos);
            pos = i + 1;
            
            if (id == RFM12_TEMPERATURE_CMD) {
				// save temperature
				temp_measurements[c] = atof(buff);
            }
            
            else if (id == RFM12_GPS_CMD) {

				if (c == 0) {
					rewrite(gps_time_tab, 13);
				}
				else if (c == 1) {
					rewrite(gps_latitude_tab, 11);
				}      
				else if (c == 2) {
					rewrite(gps_longitude_tab, 12);
				}      
				else if (c == 3) {
					rewrite(gps_speed_tab, 7);
				}      
				else if (c == 4) {
					rewrite(gps_direction_tab, 6);
				}          
            }

            else if (id == RFM12_PRESSURE_CMD) {
				if (c == 0) {
	            	mpl115a2_pressure = atof(buff);
				}     

				if (c == 1) {
	            	mpl115a2_temp = atof(buff);
				}     
            }

            else if (id == RFM12_TENSION_CMD) {
                
            }

            else if (id == RFM12_CURRENT_CMD) {
                
            }

			// id of param
			c++;
        }
	}
}

#endif
