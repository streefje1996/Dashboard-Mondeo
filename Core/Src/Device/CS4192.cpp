
#include "CS4192.hpp"

void CS4192::FillBufferOfGauge(const uint8_t& gauge, const uint16_t& value) {
	if (gauge >= NUM_GAUGES) return;
	// 0x3FF is the max 10 bit value
	if (value > 0x3FF) return;

	// Calculate buffer index and offset using 8 bits
	int buffer_index = (gauge * GAUGE_RESOLUTION) % 8;
	int offset_lsb = (gauge * GAUGE_RESOLUTION) - (8 * buffer_index);
	int remaining_bits_to_send = 8 - offset_lsb;
	int offset_msb = GAUGE_RESOLUTION - remaining_bits_to_send;

	// Fill buffer
	m_buffer[buffer_index] &= ~(0xFF << offset_lsb);
	m_buffer[buffer_index] |= value << offset_lsb;

	m_buffer[buffer_index+1] &= ~(0xFF >> offset_msb);
	m_buffer[buffer_index+1] |= value >> remaining_bits_to_send;
}

CS4192::CS4192(SPI_HandleTypeDef& hspi, GPIO_TypeDef& cs_port, const uint16_t& cs_pin, GPIO_TypeDef& oe_port, const uint16_t& oe_pin):
m_hspi(hspi), m_cs_port(cs_port), m_cs_pin(cs_pin), m_oe_port(oe_port), m_oe_pin(oe_pin), m_buffer{}
{
}

void CS4192::Init() {

	HAL_GPIO_WritePin(&m_cs_port, m_cs_pin, GPIO_PinState::GPIO_PIN_SET);
	HAL_GPIO_WritePin(&m_oe_port, m_oe_pin, GPIO_PinState::GPIO_PIN_SET);
	HAL_GPIO_WritePin(&m_cs_port, m_cs_pin, GPIO_PinState::GPIO_PIN_RESET);
}

void CS4192::SetGaugeAngle(const uint8_t &gauge, const uint16_t &angle) {

}

void CS4192::Update() {
	HAL_GPIO_WritePin(&m_cs_port, m_cs_pin, GPIO_PinState::GPIO_PIN_SET);
	HAL_SPI_Transmit_DMA(&m_hspi, m_buffer.data(), m_buffer.size());
}
