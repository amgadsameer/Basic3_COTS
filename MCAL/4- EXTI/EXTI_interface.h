#ifndef EXTI_interface_H
#define EXTI_interface_H

typedef enum
{
	INT0, 
	INT1, 
	INT2
}IntChannel_t;

typedef enum
{
	LOW_LEVEL, 
	ON_CHANGE, 
	FALLING_EDGE, 
	RISING_EDGE
}SenseCtrl_t;

void EXTI_voidInit(void);

uint8 EXTI_u8SetSenseCtrl(IntChannel_t Copy_IntCh,SenseCtrl_t Copy_SenseCtrl );

uint8 EXTI_u8EnableIntChannel(IntChannel_t Copy_IntCh);

uint8 EXTI_u8DisableIntChannel(IntChannel_t Copy_IntCh);

uint8 EXTI_u8SetCallBack(IntChannel_t Copy_IntCh, void(*Copy_pvCallBackFunc)(void));


#endif
