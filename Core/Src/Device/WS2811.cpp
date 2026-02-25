/*
 * WS2811.cpp
 *
 *  Created on: 11 Feb 2026
 *      Author: Stefan
 */

#include "Device/WS2811.hpp"
#include "etl/algorithm.h"
#include "etl/binary.h"
#include "gpio.h"
#include "Adapter/Timer_adapter.hpp"

WS2811::WS2811(TIM_HandleTypeDef &htim, const uint32_t& channel) :
m_htim{htim}, m_channel{channel}
{
}

LedInfo WS2811::GetLed(const uint8_t &LED) {
	return m_leds[LED];
}

void WS2811::SetLed(const uint8_t &LED, const LedInfo &info) {
	m_leds[LED] = info;
	update_queue.emplace(LED, info);
}

void WS2811::SetLed(const uint8_t &LED, const Color &color) {
	SetLed(LED, LedInfo{color});
}

void WS2811::SetLed(const uint8_t &LED, const uint8_t &r, const uint8_t &g, const uint8_t &b) {
	SetLed(LED, LedInfo{r, g, b});
}

void WS2811::Init() {
	etl::fill(m_buffer.begin(), m_buffer.end() - 24, T0H);

	// reset clock counter
	m_htim.Instance->CNT &= 0xFFFF0000;
	HAL_TIM_PWM_Start_DMA(&m_htim, m_channel, (uint32_t*)m_buffer.data(), m_buffer.size());
}

void WS2811::Update() {
	if(HAL_TIM_PWM_GetState(&m_htim) != HAL_TIM_STATE_READY) return;

	if(!update_queue.empty()) {

		while(!update_queue.empty()) {
			SetBufferLed(update_queue.front());
			update_queue.pop();
		}

		// reset clock counter
		m_htim.Instance->CNT &= 0xFFFF0000;
		HAL_TIM_PWM_Start_DMA(&m_htim, m_channel, (uint32_t*)m_buffer.data(), m_buffer.size());
	}
}

void WS2811::SetBufferLed(const LedWithInfo& led) {
	uint16_t index{static_cast<uint16_t>(led.first * 24)};

	for (uint8_t i = 0; i < 8; ++i) {
		m_buffer[index] = ((led.second.r << i) & 0x80) == 0 ? T0H : T1H;
		m_buffer[index + 8] = ((led.second.g << i) & 0x80) == 0 ? T0H : T1H;
		m_buffer[index + 16] = ((led.second.b << i) & 0x80) == 0 ? T0H : T1H;
		++index;
	}
}

