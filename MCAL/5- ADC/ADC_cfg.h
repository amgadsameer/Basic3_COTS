#ifndef ADC_CFG_H
#define ADC_CFG_H

/*Configure the ADC prescaler division factor, options :   1- DIVISION_BY_2
 * 														   2- DIVISION_BY_4
 * 														   3- DIVISION_BY_8
 * 														   4- DIVISION_BY_16
 * 														   5- DIVISION_BY_32
 * 														   6- DIVISION_BY_64
 * 														   7- DIVISION_BY_128
 *
 * 							*/
#define ADC_u8PRESCALER_DIVISION_FACTOR						DIVISION_BY_128

/*Configure the ADC resolution, options:		1- EIGHT_BITS
 * 												2- TEN_BITS			*/
#define ADC_u8Resolution									TEN_BITS


#endif
