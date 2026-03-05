/*
 * SSD1309.hpp
 *
 *  Created on: 2 Mar 2026
 *      Author: Stefan
 */

#ifndef INC_DEVICE_SSD1309_HPP_
#define INC_DEVICE_SSD1309_HPP_

#include <etl/array.h>
#include <etl/queue.h>
#include "Interface/ScreenInterface.hpp"
#include "main.h"
#include "Font.hpp"

class SSD1309 : public ScreenInterface {
private:
	static constexpr uint8_t s_pixel_height {64};
	static constexpr uint8_t s_collumn_height{8};
	static constexpr uint8_t s_cmd_offset{1};
	static constexpr uint8_t s_pixel_width {128};

	static constexpr uint8_t s_data_control_byte{0x40};

	// 0x00, 0xAF: 			Enable screen
	// 0x00, 0xA1: 			Remap columns
	// 0x00, 0x20, 0x00: 	Set Memory Addressing Mode to Horizontal
	// 0x00, 0xC8:			Remap rows
	static constexpr etl::array s_startup_cmd = etl::make_array<uint8_t> (0x00, 0xAF, 0x00, 0xA1,  0x00, 0x20, 0x00, 0x00, 0xC8);

	const uint8_t c_i2c_address;

	using IndexByte = etl::pair<uint16_t, uint8_t>;
	using BufferArray = etl::array<uint8_t, s_pixel_width * (s_pixel_height / s_collumn_height) + s_cmd_offset>;

	Font& m_font{Font::GetInstance()};
	bool m_enabled{false};

	I2C_HandleTypeDef& m_hi2c;

	etl::queue<IndexByte, 200> m_update_queue{};
	BufferArray m_buffer{};

	uint16_t GetIndexFromPixel(const uint16_t &x, const uint16_t &y);


public:
	SSD1309(I2C_HandleTypeDef& hi2c, const uint8_t& i2c_address);

	uint16_t GetHeight() override;
	uint16_t GetWidth() override;

	void SetPixel(const uint16_t &x, const uint16_t &y, const bool &on) override;
	void SetChar(const uint16_t &x, const uint16_t &y, const char &c) override;
	void SetString(const uint16_t &x, const uint16_t &y, const etl::string_view &msg) override;
	void SetBuffer(const etl::array_view<unsigned char> &buffer, const uint16_t &x, const uint16_t &y) override;
	void ClearScreen() override;

	void DisplayEnabledCallback();
	void DisplayUpdatedCallback();

	void Update() override;
	void EnableDisplay(const bool &enable) override;
};

#endif /* INC_DEVICE_SSD1309_HPP_ */
