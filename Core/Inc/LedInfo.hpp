/*
 * LedInfo.hpp
 *
 *  Created on: 11 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_LEDINFO_HPP_
#define INC_LEDINFO_HPP_

#include "stdint.h"


enum class Color {
	WHITE,
	RED,
	GREEN,
	BLUE,
	ORANGE,
	OFF
};

struct LedInfo {
	uint8_t r;
	uint8_t g;
	uint8_t b;

	LedInfo(const uint8_t& _r = 0, const uint8_t& _g = 0, const uint8_t& _b = 0) :
	r{_r}, g{_g}, b{_b}
	{
	}

	LedInfo(const Color& color) {
		switch (color) {
			case Color::WHITE:
				r = 255; g = 255; b = 255;
				break;
			case Color::RED:
				r = 255; g = 0; b = 0;
				break;
			case Color::GREEN:
				r = 0; g = 255; b = 0;
				break;
			case Color::BLUE:
				r = 0; g = 0; b = 255;
				break;
			case Color::ORANGE:
				r = 255; g = 132; b = 0;
				break;
			case Color::OFF:
				r = 0; g = 0; b = 0;
				break;
			default:
				r = 0; g = 0; b = 0;
				break;
		}
	}
};



#endif /* INC_LEDINFO_HPP_ */
