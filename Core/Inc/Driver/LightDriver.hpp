/*
 * LightDriver.hpp
 *
 *  Created on: 13 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DRIVER_LIGHTDRIVER_HPP_
#define INC_DRIVER_LIGHTDRIVER_HPP_

#include "Device/WS2811.hpp"
#include "Lights.hpp"
#include "etl/array.h"

class LightDriver {
private:
	WS2811 m_led_strip;

	static constexpr etl::array<uint8_t, 19> s_lights_lookup_table {
		0, //GLOW
		1, //ICE
		2, //WIPER_FLUID_LOW
		5, //BATTERY
		6, //OIL
		7, //HANDBRAKE
		8, //SEATBELT
		11, //BLINKER_LEFT
		10, //DOOR_OPEN
		9, //BLINKER_RIGHT
		12,//HIGH_BEAM
		17,//AIRBAG
		13,//CITY_LIGHTS
		16,//SERVICE
		14,//ABS
		15,//ESC
		20,//TIRES
		21,//ENGINE
		22 //FUEL
	};

public:
	LightDriver();

	LedInfo GetLight(const Lights& light);
	void SetLight(const Lights& light, const Color& color);
	void ResetLed(const Lights& light);

	void Init();
	void Update();
};

#endif /* INC_DRIVER_LIGHTDRIVER_HPP_ */
