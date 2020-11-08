#define ADC_MEASUREMENTS_CNT	32.0	// Defined variable for number of measurements

/* 
	Structure that stores the temp sum of measurements and the count of measurements
*/
typedef struct myAdc{
	uint32_t	temp_measurements;		//Temp sum of measurements
	uint8_t		measurements_count;		// Count of measurements
}myAdc_t;

/* 
	Structure for pins of PORT
*/
typedef struct myPort{
	unsigned int pin0 : 1;
	unsigned int pin1 : 1;
	unsigned int pin2 : 1;
	unsigned int pin3 : 1;
	unsigned int pin4 : 1;
	unsigned int pin5 : 1;
	unsigned int pin6 : 1;
	unsigned int pin7 : 1;
}myPort_t;

void add_adc_measurement(myAdc_t* adc, uint16_t measurement); 	// Function for adding new measurement
uint16_t calculate_adc_measurements(myAdc_t* adc);				// Function for calculating the result
uint16_t get_adc_measurement(void);								// Abstract function that return the ADC measurement
