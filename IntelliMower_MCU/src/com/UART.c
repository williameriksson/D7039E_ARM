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
	NVIC_SetPriority(USART6_IRQn, 71); //TODO: decide on prio.
	__enable_irq();

	indata_index = 0;
}

void SendIdle() {
	uint8_t sendData[1];
	uint8_t cobsData[3];

	sendData[0] = SEND_MCU_DONE;

	StuffData(sendData, 1, cobsData);
	SendData(cobsData, 3);
}

int PushData(uint8_t *data, int byte_size) {
	return 0;
}

void SendData(uint8_t *sendData, int byte_size) {
	int i = 0;
	while((i < byte_size)) {
		while(!(USART6->SR & USART_SR_TXE)) {
			//wait for transmission
		}
		USART6->DR = sendData[i];
		i++;
	}
}
