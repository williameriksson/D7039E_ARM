#include "UART.h"


uint8_t indata[RECIEVE_BUFFERSIZE];
int indata_index;

void InitUART () {
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
}


void SendStats() {
//	uint8_t byteData[5];
	double doubleData[9];
	uint8_t sendData[77]; //9 doubles, 5 chars
	uint8_t cobsData[79];

	sendData[0] = BumperFront;
	sendData[1] = BumperBack;
	sendData[2] = 0;
	sendData[3] = 0;
	sendData[4] = LiftSensor;

	doubleData[0] = (double)GetLeftSpeed();
	doubleData[1] = (double)GetRightSpeed();
	doubleData[2] = 1.0; //ultrasonic 1
	doubleData[3] = 2.0; //ultrasonic 2
	doubleData[4] = 3.0; //ultrasonic 3
	doubleData[5] = 4.0; //ultrasonic 4
	doubleData[6] = xxPos; //xcoord
	doubleData[7] = yyPos; //ycoord
	doubleData[8] = 0; //heading


	DoubleToByteArray(doubleData, 9, sendData+5);
	StuffData(sendData, 77, cobsData);
	SendData(cobsData, 79);
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
