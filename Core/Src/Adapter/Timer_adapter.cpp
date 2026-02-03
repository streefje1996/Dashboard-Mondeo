/*
 * Timer_adapter.cpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */
 

#include "Adapter/Timer_adapter.hpp"

Timer_adapter& Timer_adapter::GetInstance() {
	static Timer_adapter s_timer_adapter;
	return s_timer_adapter;
}

void Timer_adapter::SetCallback(TIM_TypeDef* timer, DelegateVoid func) {
	s_callback[timer] = func;
}

void Timer_adapter::Interupt_handler(TIM_TypeDef* timer) {
	s_callback[timer].call_if();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	Timer_adapter::Interupt_handler(htim->Instance);
}
