#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "ErrType.h"

#include "TWI_interface.h"
#include "TWI_prv.h"
#include "TWI_reg.h"
#include "TWI_cfg.h"

/*if Master is not addressed in the network, pass address 0*/
void TWI_voidMasterInit(uint8 Copy_u8Address)
{
	if(Copy_u8Address != 0u)
	{
		/*Master will be addressed in the network, assign the address*/
		TWAR = Copy_u8Address << 1u;
	}

	/*CPU freq = 16MHz, prescaler = 0, communication freq = 400kbps*/
	CLR_BIT(TWSR, TWSR_TWPS0);
	CLR_BIT(TWSR, TWSR_TWPS1);
	TWBR = 12u;

	/*Enable acknowledge*/
	SET_BIT(TWCR, TWCR_TWEA);

	/*TWI enable*/
	SET_BIT(TWCR, TWCR_TWEN);

}
void TWI_voidSlaveInit(uint8 Copy_u8Address)
{
	/*Set slave address*/
	TWAR = Copy_u8Address << 1u;

	/*TWI enable*/
	SET_BIT(TWCR, TWCR_TWEN);
}

TWI_ErrorStatus_t TWI_enuSendStartCondition(void)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	/*Set start condition bit*/
	SET_BIT(TWCR, TWCR_TWSTA);

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);

	/*wait until the previous operation finishes and the interrupt flag is raised*/
	while((GET_BIT(TWCR, TWCR_TWINT))==0u);

	/*Check the status of the bus*/
	if((TWSR & STATUS_BITS_MASK ) != START_ACK)
	{
		Local_enuError = StartCondErr;
	}
	else
	{
		/*Do nothing*/
	}

	return Local_enuError;
}
TWI_ErrorStatus_t TWI_enuSendRepeatedStart(void)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	/*Set start condition bit*/
	SET_BIT(TWCR, TWCR_TWSTA);

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);

	/*wait until the previous operation finishes and the interrupt flag is raised*/
	while((GET_BIT(TWCR, TWCR_TWINT))==0u);

	/*Check the status of the bus*/
	if((TWSR & STATUS_BITS_MASK ) != REP_START_ACK)
	{
		Local_enuError = RepStartErr;
	}
	else
	{
		/*Do nothing*/
	}

	return Local_enuError;
}


TWI_ErrorStatus_t TWI_enuSendSlaveAddressWithWrite(uint8 Copy_u8SlaveAddress)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	/*Set the slave address into the data register*/
	TWDR = Copy_u8SlaveAddress << 1u;

	/*Clear bit0 for write request*/
	CLR_BIT(TWDR,0u);

	/*Clear Start condition bit*/
	CLR_BIT(TWCR, TWCR_TWSTA);

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);

	/*wait until the previous operation finishes and the interrupt flag is raised*/
	while((GET_BIT(TWCR, TWCR_TWINT))==0u);

	/*Check the status of the bus*/
	if((TWSR & STATUS_BITS_MASK ) != SLAVE_ADD_AND_WR_ACK)
	{
		Local_enuError = SlaveAddressWithWriteErr;
	}
	else
	{
		/*Do nothing*/
	}


	return Local_enuError;
}
TWI_ErrorStatus_t TWI_enuSendSlaveAddressWithRead(uint8 Copy_u8SlaveAddress)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	/*Set the slave address into the data register*/
	TWDR = Copy_u8SlaveAddress << 1u;

	/*Set bit0 for read request*/
	SET_BIT(TWDR,0u);

	/*Clear Start condition bit*/
	CLR_BIT(TWCR, TWCR_TWSTA);

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);

	/*wait until the previous operation finishes and the interrupt flag is raised*/
	while((GET_BIT(TWCR, TWCR_TWINT))==0u);

	/*Check the status of the bus*/
	if((TWSR & STATUS_BITS_MASK ) != SLAVE_ADD_AND_RD_ACK)
	{
		Local_enuError = SlaveAddressWithReadErr;
	}
	else
	{
		/*Do nothing*/
	}

	return Local_enuError;
}
TWI_ErrorStatus_t TWI_enuMstrWriteDataByte(uint8 Copy_u8DataByte)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	/*put the data into the data register to be sent on the bus*/
	TWDR = Copy_u8DataByte;

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);

	/*wait until the previous operation finishes and the interrupt flag is raised*/
	while((GET_BIT(TWCR, TWCR_TWINT))==0u);

	/*Check the status of the bus*/
	if((TWSR & STATUS_BITS_MASK ) != MSTR_WR_BYTE_ACK)
	{
		Local_enuError = MstrWriteByteWithACKErr;
	}
	else
	{
		/*Do nothing*/
	}

	return Local_enuError;
}
TWI_ErrorStatus_t TWI_enuMstrReadDataByte(uint8* Copy_pu8DataByte)
{
	TWI_ErrorStatus_t Local_enuError = NoError;

	if(Copy_pu8DataByte != NULL)
	{

		/*Clear interrupt flag to enable slave to send the data*/
		SET_BIT(TWCR, TWCR_TWINT);

		/*wait until the previous operation finishes and the interrupt flag is raised*/
		while((GET_BIT(TWCR, TWCR_TWINT))==0u);

		/*Check the status of the bus*/
		if((TWSR & STATUS_BITS_MASK ) != MSTR_RD_BYTE_WITH_ACK)
		{
			Local_enuError = MstrReadByteWithACKErr;
		}
		else
		{
			*Copy_pu8DataByte = TWDR;
		}
	}
	else
	{
		Local_enuError = NULLPtrErr;
	}

	return Local_enuError;
}

void TWI_voidSendStopCondition(void)
{
	/*Set the stop condition bit*/
	SET_BIT(TWCR, TWCR_TWSTO);

	/*Clear interrupt flag to enable previous operation to happen*/
	SET_BIT(TWCR, TWCR_TWINT);
}
