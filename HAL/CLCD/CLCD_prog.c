#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#include "ErrType.h"

#include "DIO_interface.h"

#include "CLCD_interface.h"
#include "CLCD_prv.h"
#include "CLCD_cfg.h"

#if CLCD_u8BIT_MODE == FOUR_BIT
static void voidSetHalfDataPort(uint8 Copy_u8Data)
{
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D4_PIN, GET_BIT(Copy_u8Data,0));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D5_PIN, GET_BIT(Copy_u8Data,1));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D6_PIN, GET_BIT(Copy_u8Data,2));
	DIO_u8SetPinValue(CLCD_u8DATA_PORT,CLCD_u8D7_PIN, GET_BIT(Copy_u8Data,3));
}
#endif

static void voidSendEnablePulse(void)
{
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8E_PIN, DIO_u8PIN_HIGH);
	_delay_ms(2);
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT,CLCD_u8E_PIN, DIO_u8PIN_LOW);
}

void CLCD_voidSendCmd(uint8 Copy_u8Cmd)
{
	/*Set RS pin to low for command*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN, DIO_u8PIN_LOW);

	/*Set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS == DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN, DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/*Send the command*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Cmd);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/*Send the 4 Most significant bits of the command first*/
	voidSetHalfDataPort(Copy_u8Cmd >> 4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Cmd);
	voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration option
#endif

}

void CLCD_voidSendData(uint8 Copy_u8Data)
{
	/*Set RS pin to high for data*/
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RS_PIN, DIO_u8PIN_HIGH);

	/*Set RW pin to low for writing*/
#if CLCD_u8RW_CONN_STS == DIO_CONNECTED
	DIO_u8SetPinValue(CLCD_u8CTRL_PORT, CLCD_u8RW_PIN, DIO_u8PIN_LOW);
#endif

#if CLCD_u8BIT_MODE == EIGHT_BIT
	/*Send the Data*/
	DIO_u8SetPortValue(CLCD_u8DATA_PORT,Copy_u8Data);
	voidSendEnablePulse();

#elif CLCD_u8BIT_MODE == FOUR_BIT
	/*Send the 4 Most significant bits of the data first*/
	voidSetHalfDataPort(Copy_u8Data >> 4);
	voidSendEnablePulse();
	voidSetHalfDataPort(Copy_u8Data);
	voidSendEnablePulse();

#else
#error wrong CLCD_u8BIT_MODE configuration option
#endif
}

void CLCD_voidInit(void)
{
	/*Wait for more than 30 ms after power on*/
	_delay_ms(40);

	/*Function set command: 2 lines, Font size: 5x7*/
#if CLCD_u8BIT_MODE == EIGHT_BIT
	CLCD_voidSendCmd(0b00111000);

#elif  CLCD_u8BIT_MODE == FOUR_BIT
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b0010);
	voidSendEnablePulse();
	voidSetHalfDataPort(0b1000);
	voidSendEnablePulse();

#endif

	/*Display on off control : Display on, cursor off, blink cursor off*/
	CLCD_voidSendCmd(0b00001100);

	/*Clear display*/
	CLCD_voidSendCmd(1);

}

uint8 CLCD_u8SendString(const char* Copy_pchString)
{
	uint8 Local_u8ErrorState = OK;
	if(Copy_pchString != NULL)
	{
		uint8 Local_u8Iterator=0u;
		while(Copy_pchString[Local_u8Iterator] != '\0')
		{
			CLCD_voidSendData(Copy_pchString[Local_u8Iterator]);
			Local_u8Iterator++;
		}
	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}

void CLCD_voidSendNumber(sint32 Copy_s32Number)
{
	char Local_chNumber[10];
	uint8 Local_u8RightDigit, Local_u8Counter1=0u;
	sint8 Local_s8Counter2;

	if(Copy_s32Number ==0)
	{
		CLCD_voidSendData('0');
		return ;
	}
	else if(Copy_s32Number < 0)
	{
		/*Number is -ve, make it positive, print -ve sign on the CLCD*/
		Copy_s32Number *= -1;

		CLCD_voidSendData('-');
	}

	while(Copy_s32Number != 0)
	{
		Local_u8RightDigit = (uint8)((uint32)Copy_s32Number % 10); /*Get the right most digit*/

		Copy_s32Number /= 10;	/*remove the right most digit*/

		Local_chNumber[Local_u8Counter1] = Local_u8RightDigit + '0'; /*Convert to to ASCII*/

		Local_u8Counter1++;
	}

	for(Local_s8Counter2 = (sint8)Local_u8Counter1 -1 ; Local_s8Counter2 >= 0 ; Local_s8Counter2--)
	{
		CLCD_voidSendData(Local_chNumber[(uint8)Local_s8Counter2]);
	}

}

void CLCD_voidGoToXY(uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8Address ;
	if(Copy_u8YPos == 0u)
	{
		Local_u8Address = Copy_u8XPos;
	}
	else if(Copy_u8YPos == 1u)
	{
		Local_u8Address = 0x40 + Copy_u8XPos;
	}
	// Local_u8Address = 0x40 * Copy_u8YPos + Copy_u8XPos;

	/*Set bit 7 for SetDDRAM Address command*/
	SET_BIT(Local_u8Address,7);

	/*Execute Set DDRAM Address command*/
	CLCD_voidSendCmd(Local_u8Address);
}

uint8 CLCD_u8SendSpecialCharacter(uint8 Copy_u8LocationNum, uint8* Copy_pu8Pattern, uint8 Copy_u8XPos, uint8 Copy_u8YPos)
{
	uint8 Local_u8ErrorState = OK;

	if(Copy_pu8Pattern != NULL)
	{
		uint8 Local_u8Counter;

		uint8 Local_u8CGRAMAddress = Copy_u8LocationNum * 8;

		/*Set bit 6 for CGRAM Address command*/
		SET_BIT(Local_u8CGRAMAddress , 6u);

		/*Set ACGRAM Address*/
		CLCD_voidSendCmd(Local_u8CGRAMAddress);

		/*Write the input pattern inside CGRAM*/
		for(Local_u8Counter=0u; Local_u8Counter<8u; Local_u8Counter++)
		{
			CLCD_voidSendData(Copy_pu8Pattern[Local_u8Counter]);
		}

		/*Go back to DDRAM*/
		CLCD_voidGoToXY(Copy_u8XPos, Copy_u8YPos);

		/*Display the special pattern inside CGRAM*/
		CLCD_voidSendData(Copy_u8LocationNum);

	}
	else
	{
		Local_u8ErrorState = NULL_PTR_ERR;
	}

	return Local_u8ErrorState;
}
