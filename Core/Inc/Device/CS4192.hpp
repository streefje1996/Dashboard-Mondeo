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
#include "Adapter/Spi_adapter.hpp"
#include <math.h>
#include <etl/array.h>
#include <etl/rounded_integral_division.h>

// Template for number of gauges
template<uint8_t N>
class CS4192 {
private:
	static constexpr uint8_t s_gaugeResolution{10};
	static constexpr float s_resolutionPerAngle{1023.f/360.f};

	bool m_isSpiBusy{false};

	SPI_HandleTypeDef& m_hspi;

	GPIO_TypeDef* m_cs_port;
	const uint16_t c_cs_pin;

	GPIO_TypeDef* m_oe_port;
	const uint16_t c_oe_pin;

	// 5 bytes = 40bits = 10 bits * 4 Gauges
	etl::array<uint8_t, etl::divide_round_to_ceiling(N * s_gaugeResolution, 8)> m_buffer;
	etl::array<uint8_t, etl::divide_round_to_ceiling(N * s_gaugeResolution, 8)> m_reverse_buffer;

	void FillBufferOfGauge(const uint8_t& gauge, const uint16_t& value) {
		if (gauge >= N) return;
		// 0x3FF is the max 10 bit value
		if (value > 0x3FF) return;

		// Calculate buffer index and offset using 8 bits
		int buffer_index = (gauge * s_gaugeResolution) / 8;
		int offset_lsb_first_half = (gauge * s_gaugeResolution) - (8 * buffer_index);
		int bits_in_first_half = 8 - offset_lsb_first_half;
		int offset_lsb_second_half = s_gaugeResolution - bits_in_first_half;

		// Fill buffer
		m_buffer[buffer_index] &= ~(0xFF << offset_lsb_first_half);
		m_buffer[buffer_index] |= value << offset_lsb_first_half;

		m_buffer[buffer_index+1] &= 0xFF << offset_lsb_second_half;
		m_buffer[buffer_index+1] |= value >> bits_in_first_half;
	}

public:
	CS4192(SPI_HandleTypeDef& hspi, GPIO_TypeDef* cs_port, const uint16_t& cs_pin, GPIO_TypeDef* oe_port, const uint16_t& oe_pin):
	m_hspi{hspi}, m_cs_port{cs_port}, c_cs_pin{cs_pin}, m_oe_port{oe_port}, c_oe_pin{oe_pin}, m_buffer{}, m_reverse_buffer{}
	{
		Spi_adaptor::GetInstance().SetCallback(etl::delegate<void(void)>::create<CS4192, &CS4192::TxCpltCallback>(*this));
	}

	void Init() {
		HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_SET);
		HAL_GPIO_WritePin(m_oe_port, c_oe_pin, GPIO_PinState::GPIO_PIN_SET);
		HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_RESET);
	}

	void Update() {
		if (m_isSpiBusy) return;
		m_isSpiBusy = true;
		HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_SET);

		etl::reverse_copy(m_buffer.cbegin(), m_buffer.cend(), m_reverse_buffer.begin());
		HAL_SPI_Transmit_DMA(&m_hspi, m_reverse_buffer.data(), m_reverse_buffer.size());
	}

	void SetGaugeAngle(const uint8_t& gauge, const float& angle) {
		if (gauge >= N) return;
		if (angle > 360) return;

		FillBufferOfGauge(gauge, angle * s_resolutionPerAngle);
	}

	void TxCpltCallback() {
		HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_RESET);
		m_isSpiBusy = false;
	}
};



#endif /* INC_DEVICE_CS4192_HPP_ */
