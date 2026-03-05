/*
 * ScreenInterface.hpp
 *
 *  Created on: 2 Mar 2026
 *      Author: Stefan
 */

#ifndef INC_INTERFACE_SCREENINTERFACE_HPP_
#define INC_INTERFACE_SCREENINTERFACE_HPP_

#include "etl/array_view.h"
#include "etl/string_view.h"

class ScreenInterface {
public:
	virtual uint16_t GetWidth() = 0;
	virtual uint16_t GetHeight() = 0;

	virtual void SetPixel(const uint16_t& x, const uint16_t& y, const bool& on = true) = 0;
	virtual void SetChar(const uint16_t& x, const uint16_t& y, const char& c) = 0;
	virtual void SetString(const uint16_t& x, const uint16_t& y, const etl::string_view& msg) = 0;
	virtual void SetBuffer(const etl::array_view<uint8_t>& buffer, const uint16_t& x = 0, const uint16_t& y = 0) = 0;
	virtual void ClearScreen() = 0;

	virtual void EnableDisplay(const bool& enable = true) = 0;
	virtual void Update() = 0;
};



#endif /* INC_INTERFACE_SCREENINTERFACE_HPP_ */
