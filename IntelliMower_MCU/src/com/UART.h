#ifndef COM_UART_H_
#define COM_UART_H_

#include "stm32f411xe.h"
#include "utils/Cobs.h"
#include "utils/convertions/floatLib.h"

#define BAUDRATE 9600
#define RECIEVE_BUFFERSIZE 80
#define SEND_DATA 0x00
#define SEND_MCU_DONE 0x01

void InitUART();
void SendIdle();
void SendData(uint8_t *sendData, int byte_size);

#endif /* COM_UART_H_ */
