/*
 * Uart_adapter.hpp
 *
 *  Created on: 8 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_ADAPTER_UART_ADAPTER_HPP_
#define INC_ADAPTER_UART_ADAPTER_HPP_

#include <etl/delegate.h>
#include <etl/flat_map.h>
#include "main.h"

class Uart_adapter {
public:
	using DelegateFunc = etl::delegate<void(const uint16_t&)>;
	using DelegateMap = etl::flat_map<USART_TypeDef*, DelegateFunc, 1>;

	static Uart_adapter& GetInstance();

	void SetCallback(USART_TypeDef* usart, const DelegateFunc& func);
	static void Interupt_handler(USART_TypeDef* usart, const uint16_t& size);

private:
	static inline DelegateMap s_callback;

	Uart_adapter() = default;
	~Uart_adapter() = default;

	Uart_adapter(Uart_adapter&) = delete;
	void operator=(const Uart_adapter&) = delete;
};



#endif /* INC_ADAPTER_UART_ADAPTER_HPP_ */
