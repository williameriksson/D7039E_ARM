#include "UART.h"


uint8_t indata[RECIEVE_BUFFERSIZE];
int indata_index;

void initUART () {
	__disable_irq();
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
	GPIOC->MODER |= GPIO_MODER_MODER7_1 | GPIO_MODER_MODER6_1;
	GPIOC->AFR[0] |= (((uint8_t)0x08) << 24);
	GPIOC->AFR[0] |= (((uint8_t)0x08) << 28);
	USART6->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART6->CR1 |= USART_CR1_RXNEIE; //Enable UART RXNE Interrupt
	USART6->CR1 |= USART_CR1_UE; // Enable USART, word length defaults to 8 bits and 1 stop bit
	USART6->CR1 |= USART_CR1_TE;
	USART6->BRR |= 50000000L/BAUDRATE;

	NVIC_EnableIRQ(USART6_IRQn);
	NVIC_SetPriority(USART6_IRQn, 71);
	__enable_irq();

//	circularBufferInit(&recieveBuffer, (uint8_t)0, (uint8_t)RECIEVE_BUFFERSIZE);
//	fillBuffer(&recieveBuffer, 0);
	indata_index = 0;
}

void USART6_IRQHandler (void) {
	uint8_t ch;
	if(USART6->SR & USART_SR_RXNE) {
		ch = (uint8_t) USART6->DR;
//		USART6->SR &= ~USART_SR_RXNE;
//		if((int)ch != (int)0x1) {
//			indata[indata_index] = ch;
//			indata_index++;
//		}
//		else {
//			indata[indata_index] = ch;
//			uint8_t decoded[indata_index-1];
////			unStuffData(indata, indata_index+1, decoded);
////			runCommand(decoded);
//			indata_index = 0;
//		}
		SendStats();
	}
//	else if(USART6->SR & USART_SR_TXE) {
//		USART6->DR = ch;
//	}
}

void SendStats() {
	float floatData[3];
	uint8_t byteData[12];
	uint8_t sendData[14];

//	floatData[0] = leftEncoder.encoderDistance;
//	floatData[1] = rightEncoder.encoderDistance;

	floatData[0] = 1.0f;
	floatData[1] = 0.0f;
	floatData[2] = 0.123456f;

	FloatToByteArray(floatData, 3, byteData);
	StuffData(byteData, 12, sendData);

	SendData(sendData, 14);
}

void SendData(uint8_t *sendData, int byte_size) {
	int i = 0;
	while((i < byte_size)) {
		USART6->DR = sendData[i];
		i++;
		while(!(USART6->SR & USART_SR_TXE)) {
			//wait for transmission
		}
	}
}
