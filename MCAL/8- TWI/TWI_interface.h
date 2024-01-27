#ifndef TWI_INTERFACE_H
#define TWI_INTERFACE_H

typedef enum
{
	NoError,
	NULLPtrErr,
	StartCondErr,
	RepStartErr,
	SlaveAddressWithWriteErr,
	SlaveAddressWithReadErr,
	MstrWriteByteWithACKErr,
	MstrReadByteWithACKErr	
	
}TWI_ErrorStatus_t;

/*if Master is not addressed in the network, pass address 0*/
void TWI_voidMasterInit(uint8 Copy_u8Address);

void TWI_voidSlaveInit(uint8 Copy_u8Address);

TWI_ErrorStatus_t TWI_enuSendStartCondition(void);
TWI_ErrorStatus_t TWI_enuSendRepeatedStart(void);
TWI_ErrorStatus_t TWI_enuSendSlaveAddressWithWrite(uint8 Copy_u8SlaveAddress);
TWI_ErrorStatus_t TWI_enuSendSlaveAddressWithRead(uint8 Copy_u8SlaveAddress);
TWI_ErrorStatus_t TWI_enuMstrWriteDataByte(uint8 Copy_u8DataByte);
TWI_ErrorStatus_t TWI_enuMstrReadDataByte(uint8* Copy_pu8DataByte);

void TWI_voidSendStopCondition(void);


#endif
