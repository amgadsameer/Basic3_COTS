#ifndef TIMERS_INTERFACE_H
#define TIMERS_INTERFACE_H

void Timer0_voidInit(void);

uint8 Timer0_u8SetCallBack(void(*Copy_pvCallBackFunc)(void));

void Timer0_voidSetPreloadValue(uint8 Copy_u8PreloadVal);

#endif