/*
 * Uart_adapter.cpp
 *
 *  Created on: 8 Feb 2026
 *      Author: Stefan
 */

#include "Adapter/Uart_adapter.hpp"

Uart_adapter& Uart_adapter::GetInstance() {
	static Uart_adapter uart_adapter;
	return uart_adapter;
}

void Uart_adapter::SetCallback(USART_TypeDef *usart, const DelegateFunc &func) {
	s_callback[usart] = func;
}

void Uart_adapter::Interupt_handler(USART_TypeDef *usart, const uint16_t &size) {
	s_callback[usart].call_if(size);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	if (huart->RxEventType == HAL_UART_RXEVENT_HT) return;
	Uart_adapter::Interupt_handler(huart->Instance, Size);
}




