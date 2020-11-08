#include <funcs.h>

/* 	
	This function adds the new ADC measurement to the sum of the previous measurements
	Then the number of measurements increases
*/
void add_adc_measurement(myAdc_t* adc, uint32_t measurement){
	adc->temp_measurements += measurement;
	adc->measurements_count++;
}

/* 	
	This function return the calculated average value of all ADC measurements
*/
uint16_t calculate_adc_measurements(myAdc_t* adc){
	uint16_t result = adc->temp_measurements/ADC_MEASUREMENTS_CNT
	adc->measurements_count = 0;	// Reset count of measurements
	adc->temp_measurements = 0;		// Reset temp measurements sum
	return result;
}

/* 	
	Abstract function that return the ADC measurement
*/
uint16_t get_adc_measurement(void){
	return 0;
}

