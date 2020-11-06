#include <funcs.h>

/* 	
	This function adds the new ADC measurement to the sum of the previous measurements
	Then the number of measurements increases
*/
void add_adc_measure(myAdc_t* adc, uint32_t measure){
	adc->temp_measurmtnts += measure;
	adc->measuremnts_count++;
}

/* 	
	This function return the calculated average value of all ADC measurements
*/
double calculate_adc_measurements(myAdc_t* adc){
	adc->measuremnts_count = 0;	// Reset count of measurements
	return (adc->temp_measurmtnts/ADC_MEASURES_CNT);
}

/* 	
	Abstract function that return the ADC measurement
*/
uint32_t get_adc_measure(void){
	return 0;
}

