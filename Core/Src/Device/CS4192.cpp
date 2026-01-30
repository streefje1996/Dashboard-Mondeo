
#include "CS4192.hpp"

#include "Adapter/Spi_adapter.hpp"

void CS4192::FillBufferOfGauge(const uint8_t& gauge, const uint16_t& value) {
	if (gauge >= c_numGauges) return;
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

CS4192::CS4192(SPI_HandleTypeDef& hspi, GPIO_TypeDef* cs_port, const uint16_t& cs_pin, GPIO_TypeDef* oe_port, const uint16_t& oe_pin):
m_hspi(hspi), m_cs_port(cs_port), c_cs_pin(cs_pin), m_oe_port(oe_port), c_oe_pin(oe_pin), m_buffer{}
{
	Spi_adaptor::GetInstance().SetCallback(etl::delegate<void(void)>::create<CS4192, &CS4192::TxCpltCallback>(*this));
}

void CS4192::Init() {

	HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_SET);
	HAL_GPIO_WritePin(m_oe_port, c_oe_pin, GPIO_PinState::GPIO_PIN_SET);
	HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_RESET);
}

void CS4192::SetGaugeAngle(const uint8_t &gauge, const float &angle) {
	if (gauge >= c_numGauges) return;
	if (angle > 360) return;

	FillBufferOfGauge(gauge, angle * s_resolutionPerAngle);
}

void CS4192::Update() {
	if (m_isSpiBusy) return;
	m_isSpiBusy = true;
	HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_SET);

	etl::reverse_copy(m_buffer.cbegin(), m_buffer.cend(), m_reverse_buffer.begin());
	HAL_SPI_Transmit_DMA(&m_hspi, m_reverse_buffer.data(), m_reverse_buffer.size());
}

void CS4192::TxCpltCallback() {
	HAL_GPIO_WritePin(m_cs_port, c_cs_pin, GPIO_PinState::GPIO_PIN_RESET);
	m_isSpiBusy = false;
}
