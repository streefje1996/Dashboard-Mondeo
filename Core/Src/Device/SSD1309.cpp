/*
 * SSD1309.cpp
 *
 *  Created on: 2 Mar 2026
 *      Author: Stefan
 */

#include "SSD1309.hpp"
#include "Adapter/I2c_adapter.hpp"

SSD1309::SSD1309(I2C_HandleTypeDef& hi2c, const uint8_t& i2c_address) :
c_i2c_address{i2c_address}, m_hi2c{hi2c} {
}

void SSD1309::ClearScreen() {
	m_buffer.fill(0x00);
	m_buffer[0] = s_data_control_byte;
}

void SSD1309::SetChar(const uint16_t &x, const uint16_t &y, const char &c) {

	uint8_t offset_from_lsb = y % s_collumn_height;
	uint16_t index = GetIndexFromPixel(x, y);

	for (auto& byte : m_font[c]) {


		if (offset_from_lsb > 0) {
			// needs to be offset
			m_update_queue.push(IndexByte{index, byte<<offset_from_lsb});
			m_update_queue.push(IndexByte{index + s_pixel_width, byte >> (s_collumn_height - offset_from_lsb)});
			++index;
		} else {
			// no offset needed
			m_update_queue.push(IndexByte{index++, byte});
		}
	}
}

uint16_t SSD1309::GetWidth() {
	return s_pixel_width;
}

void SSD1309::SetBuffer(const etl::array_view<unsigned char> &buffer, const uint16_t &x, const uint16_t &y) {
}

uint16_t SSD1309::GetHeight() {
	return s_pixel_height;
}

void SSD1309::SetPixel(const uint16_t &x, const uint16_t &y, const bool &on) {
	uint8_t offset_from_lsb = y % s_collumn_height;
	m_update_queue.push(IndexByte{GetIndexFromPixel(x,y), on << offset_from_lsb});
}

void SSD1309::Update() {
	if(!m_enabled) return;
	if(HAL_I2C_GetState(&m_hi2c) !=  HAL_I2C_STATE_READY) return;
	if(m_update_queue.empty()) return;

	while(!m_update_queue.empty()) {
		auto data = m_update_queue.front();
		m_buffer[data.first] |= data.second;
		m_update_queue.pop();
	}

	HAL_I2C_Master_Transmit_DMA(&m_hi2c, c_i2c_address, m_buffer.data(), m_buffer.SIZE);
}

void SSD1309::DisplayEnabledCallback() {
	m_enabled =  true;

	auto func = I2c_adapter::DelegateVoid::create<SSD1309, &SSD1309::DisplayUpdatedCallback>(*this);
	I2c_adapter::GetInstance().SetDmaCompletedCallback(m_hi2c.Instance, func);

	HAL_I2C_Master_Transmit_DMA(&m_hi2c, c_i2c_address, m_buffer.data(), m_buffer.SIZE);
}

void SSD1309::DisplayUpdatedCallback() {
}

uint16_t SSD1309::GetIndexFromPixel(const uint16_t &x, const uint16_t &y) {
	uint8_t row = y / 8;
	return row * s_pixel_width + x + s_cmd_offset;
}

void SSD1309::EnableDisplay(const bool &enable) {
	if(HAL_I2C_GetState(&m_hi2c) !=  HAL_I2C_STATE_READY) return;

	ClearScreen();

	auto func = I2c_adapter::DelegateVoid::create<SSD1309, &SSD1309::DisplayEnabledCallback>(*this);
	I2c_adapter::GetInstance().SetDmaCompletedCallback(m_hi2c.Instance, func);

	HAL_I2C_Master_Transmit_DMA(&m_hi2c, c_i2c_address, (uint8_t*)s_startup_cmd.data(), s_startup_cmd.SIZE);
}

void SSD1309::SetString(const uint16_t &x, const uint16_t &y, const etl::string_view &msg) {
	uint16_t new_x = x;
	for (auto& c : msg) {
		SetChar(new_x, y, c);
		new_x += 8;
	}
}
