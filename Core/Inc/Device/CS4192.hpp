/*
 * CS4192.hpp
 *
 *  Created on: 29 Jan 2026
 *      Author: Stefan
 */

#ifndef INC_DEVICE_CS4192_HPP_
#define INC_DEVICE_CS4192_HPP_

#include "spi.h"
#include "gpio.h"
#include <math.h>
#include <etl/array.h>

class CS4192 {
private:
	static constexpr uint8_t s_gaugeResolution{10};
	static constexpr float s_resolutionPerAngle{1023/360};

	const uint8_t c_numGauges{4};

	bool m_isSpiBusy{false};

	SPI_HandleTypeDef& m_hspi;

	GPIO_TypeDef& m_cs_port;
	const uint16_t c_cs_pin;

	GPIO_TypeDef& m_oe_port;
	const uint16_t c_oe_pin;

	// 40 = 10 Bits * 4 Gauges
	etl::array<uint8_t, 40> m_buffer;

	void FillBufferOfGauge(const uint8_t& gauge, const uint16_t& value);

public:
	CS4192(SPI_HandleTypeDef& hspi, GPIO_TypeDef& cs_port, const uint16_t& cs_pin, GPIO_TypeDef& oe_port, const uint16_t& oe_pin);

	void Init();
	void Update();

	void SetGaugeAngle(const uint8_t& gauge, const float& angle);
	void TxCpltCallback();
};



#endif /* INC_DEVICE_CS4192_HPP_ */
