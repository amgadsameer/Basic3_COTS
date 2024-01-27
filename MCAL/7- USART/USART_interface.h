#ifndef USART_INTERFACE_H
#define USART_INTERFACE_H

void USART_voidInit(void);

void USART_voidTransmit(uint8 Copy_u8Data);

uint8 USART_u8Receive(void);

#endif