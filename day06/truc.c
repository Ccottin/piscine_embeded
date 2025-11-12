#include "lib_i2c.h"

volatile uint8_t     status;

void explain_status(void)
{
	switch (status)
	{
	case TW_START:
		uart_printstr("start condition transmitted");
		break;

	case TW_REP_START:
		uart_printstr("repeated start condition transmitted");
		break;

	case TW_MT_SLA_ACK:
		uart_printstr("SLA+W transmitted, ACK received");
		break;

	case TW_MT_SLA_NACK:
		uart_printstr("SLA+W transmitted, NACK received");
		break;

	case TW_MT_DATA_ACK:
		uart_printstr("data transmitted, ACK received");
		break;

	case TW_MT_DATA_NACK:
		uart_printstr("data transmitted, NACK received");
		break;

	case TW_MT_ARB_LOST:
		uart_printstr("arbitration lost in SLA+W or data");
		break;

	case TW_MR_SLA_ACK:
		uart_printstr("SLA+R transmitted, ACK received");
		break;

	case TW_MR_SLA_NACK:
		uart_printstr("SLA+R transmitted, NACK received");
		break;

	case TW_MR_DATA_ACK:
		uart_printstr("data received, ACK returned");
		break;

	case TW_MR_DATA_NACK:
		uart_printstr("data received, NACK returned");
		break;

	case TW_ST_SLA_ACK:
		uart_printstr("SLA+R received, ACK returned");
		break;

	case TW_ST_ARB_LOST_SLA_ACK:
		uart_printstr("arbitration lost in SLA+RW, SLA+R received, ACK returned");
		break;

	case TW_ST_DATA_ACK:
		uart_printstr("data transmitted, ACK received");
		break;

	case TW_ST_DATA_NACK:
		uart_printstr("data transmitted, NACK received");
		break;

	case TW_ST_LAST_DATA:
		uart_printstr("last data byte transmitted, ACK received");
		break;

	case TW_SR_SLA_ACK:
		uart_printstr("SLA+W received, ACK returned");
		break;

	case TW_SR_ARB_LOST_SLA_ACK:
		uart_printstr("arbitration lost in SLA+RW, SLA+W received, ACK returned");
		break;

	case TW_SR_GCALL_ACK:
		uart_printstr("general call received, ACK returned");
		break;

	case TW_SR_ARB_LOST_GCALL_ACK:
		uart_printstr("arbitration lost in SLA+RW, general call received, ACK returned");
		break;

	case TW_SR_DATA_ACK:
		uart_printstr("data received, ACK returned");
		break;

	case TW_SR_DATA_NACK:
		uart_printstr("data received, NACK returned");
		break;

	case TW_SR_GCALL_DATA_ACK:
		uart_printstr("general call data received, ACK returned");
		break;

	case TW_SR_GCALL_DATA_NACK:
		uart_printstr("general call data received, NACK returned");
		break;

	case TW_SR_STOP:
		uart_printstr("stop or repeated start condition received while selected");
		break;

	case TW_NO_INFO:
		uart_printstr("no state information available");
		break;

	case TW_BUS_ERROR:
		uart_printstr("illegal start or stop condition");
		break;
	}
}