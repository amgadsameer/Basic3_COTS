#ifndef ADC_INTERFACE_H
#define ADC_INTERFACE_H


void ADC_voidInit(void);

uint8 ADC_u8StartConversionSynch(uint8 Copy_u8Channel, uint16* Copy_pu16Result);

uint8 ADC_u8StartConversionAsynch(uint8 Copy_u8Channel, uint16* Copy_pu16Result, void(*Copy_pvNotificationFunc)(void));

#endif
