/*
 * Timer_adapter.hpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */

#ifndef SRC_ADAPTER_TIMER_ADAPTER_HPP_
#define SRC_ADAPTER_TIMER_ADAPTER_HPP_

#include <etl/delegate.h>
#include <etl/flat_map.h>
#include "main.h"

class Timer_adapter {
public:
	using DelegateVoid = etl::delegate<void(void)>;
	using DelegateMap = etl::flat_map<TIM_TypeDef*, DelegateVoid, 1>;

	static Timer_adapter& GetInstance();

	void SetCallback(TIM_TypeDef* timer, DelegateVoid func);
	static void Interupt_handler(TIM_TypeDef* timer);

private:
	static inline DelegateMap s_callback;

	Timer_adapter() = default;
	~Timer_adapter() = default;

	Timer_adapter(Timer_adapter&) = delete;
	void operator=(const Timer_adapter&) = delete;
};



#endif /* SRC_ADAPTER_TIMER_ADAPTER_HPP_ */
