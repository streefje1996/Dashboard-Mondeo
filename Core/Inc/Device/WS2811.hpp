/*
 * WS2811.hpp
 *
 *  Created on: 11 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DEVICE_WS2811_HPP_
#define INC_DEVICE_WS2811_HPP_

#include "etl/array.h"
#include "LedInfo.hpp"
#include "tim.h"
#include "main.h"
#include "etl/queue.h"


class WS2811 {
private:
	// 22 LEDs total
	using LedArray = etl::array<LedInfo, 23>;
	// 2 bytes per bit, 24 bits per LED
	using BufferArray = etl::array<uint16_t, (LedArray::SIZE + 1) * 24>;

	using LedWithInfo = etl::pair<const uint8_t, const LedInfo>;

	using UpdateQueue = etl::queue<LedWithInfo, 22, etl::memory_model::MEMORY_MODEL_LARGE>;

	static constexpr uint8_t T0H{8};
	// is actually 19.2
	static constexpr uint8_t T1H{19};

	LedArray m_leds{};
	BufferArray m_buffer{};
	UpdateQueue update_queue{};

	TIM_HandleTypeDef& m_htim;
	const uint32_t m_channel;

	void SetBufferLed(const LedWithInfo& led);

public:
	WS2811(TIM_HandleTypeDef& htim, const uint32_t& channel);


	LedInfo GetLed(const uint8_t& LED);
	void SetLed(const uint8_t& LED, const LedInfo& info);
	void SetLed(const uint8_t& LED, const Color& color);
	void SetLed(const uint8_t& LED, const uint8_t& r = 255, const uint8_t& g = 255, const uint8_t& b = 255);

	void Init();
	void Update();

};



#endif /* INC_DEVICE_WS2811_HPP_ */
