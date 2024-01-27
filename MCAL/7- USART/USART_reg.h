#ifndef USART_REG_H
#define USART_REG_H

#define UDR				*((volatile uint8*)0x2C)
		
#define UCSRA			*((volatile uint8*)0x2B)
#define UCSRA_RXC		7u 			/*Receive complete flag*/
#define UCSRA_TXC		6u			/*Transmit complete flag*/
#define UCSRA_UDRE		5u			/*Data egister empty flag*/

#define UCSRB			*((volatile uint8*)0x2A)
#define UCSRB_RXEN		4u			/*Receiver enable*/
#define UCSRB_TXEN		3u			/*Transmitter enable*/

#define UCSRC			*((volatile uint8*)0x40)
#define UCSRC_URSEL		7u 							/*Register select*/
#define UCSRC_UMSEL		6u                          /*Mode select*/
#define UCSRC_UPM1		5u                          /*Parity mode select bit 1*/
#define UCSRC_UPM0		4u                          /*Parity mode select bit 0*/
#define UCSRC_USBS		3u                          /*Stop bit select*/
#define UCSRC_UCSZ1		2u                          /*Character size bit 1*/
#define UCSRC_UCSZ0		1u                          /*Character size bit 0*/
#define UCSRC_UCPOL		0u                          /*Clock polarity*/

#define UBRRH			*((volatile uint8*)0x40)
#define UBRRL			*((volatile uint8*)0x29)

#endif