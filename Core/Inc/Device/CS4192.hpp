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
#include <array>
#include <limits.h>

#define NUM_GAUGES 4
#define GAUGE_RESOLUTION 10

constexpr uint8_t buffer_size = std::ceil((NUM_GAUGES * GAUGE_RESOLUTION) / 8);

class CS4192 {
private:
	SPI_HandleTypeDef& m_hspi;
	GPIO_TypeDef& m_cs_port;
	const uint16_t m_cs_pin;
	GPIO_TypeDef& m_oe_port;
	const uint16_t m_oe_pin;

	std::array<uint8_t, buffer_size> m_buffer;

	void FillBufferOfGauge(const uint8_t& gauge, const uint16_t& value);

public:
	CS4192(SPI_HandleTypeDef& hspi, GPIO_TypeDef& cs_port, const uint16_t& cs_pin, GPIO_TypeDef& oe_port, const uint16_t& oe_pin);

	void Init();

	void SetGaugeAngle(const uint8_t& gauge, const uint16_t& angle);

	void Update();

};



#endif /* INC_DEVICE_CS4192_HPP_ */
