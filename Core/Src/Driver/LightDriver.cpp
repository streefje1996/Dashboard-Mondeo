/*
 * LightDriver.cpp
 *
 *  Created on: 13 Feb 2026
 *      Author: Stefan
 */

#include "Driver/LightDriver.hpp"

LightDriver::LightDriver() :
m_led_strip{htim3, TIM_CHANNEL_1}
{
}

LedInfo LightDriver::GetLight(const Lights &light) {
	return m_led_strip.GetLed(s_lights_lookup_table[static_cast<uint8_t>(light)]);
}

void LightDriver::SetLight(const Lights &light, const Color &color) {
	if (light==Lights::BACKLIGHT) {
		m_led_strip.SetLed(3, color);
		m_led_strip.SetLed(4, color);
		m_led_strip.SetLed(18, color);
		m_led_strip.SetLed(19, color);
		return;
	}
	m_led_strip.SetLed(s_lights_lookup_table[static_cast<uint8_t>(light)], color);
}

void LightDriver::ResetLed(const Lights &light) {
	if (light==Lights::BACKLIGHT) {
		m_led_strip.SetLed(19, Color::OFF);
		m_led_strip.SetLed(4, Color::OFF);
		m_led_strip.SetLed(18, Color::OFF);
		m_led_strip.SetLed(19, Color::OFF);
		return;
	}
	m_led_strip.SetLed(s_lights_lookup_table[static_cast<uint8_t>(light)], Color::OFF);
}

void LightDriver::Init() {
	m_led_strip.Init();
}

void LightDriver::Update() {
	m_led_strip.Update();
}
