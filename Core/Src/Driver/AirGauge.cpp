/*
 * AirGauge.cpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */

#include "Driver/AirGauge.hpp"

AirGauge::AirGauge():
m_cs4192{hspi1, SPI1_CS_GPIO_Port, SPI1_CS_Pin, ACG_OE_GPIO_Port, ACG_OE_Pin}, m_gauge_settings{}
{
	DefaultGaugesConfig();
}

void AirGauge::CalibrateGauge(const uint8_t& gauge, const float& zero_angle, const float& min_angle, const float& max_angle) {
	if (gauge > s_num_gauges) return;
	m_gauge_settings[gauge].zero_angle = zero_angle;
	m_gauge_settings[gauge].min_angle = min_angle;
	m_gauge_settings[gauge].max_angle = max_angle;
}

float AirGauge::GetAngle(const uint8_t& gauge) const {
	if (gauge > s_num_gauges) return 0;
	return m_gauge_settings[gauge].current_angle;
}

void AirGauge::SetAngle(const uint8_t& gauge, const float& angle) {
	if (gauge > s_num_gauges) return;

	if (angle < m_gauge_settings[gauge].min_angle) {
		m_gauge_settings[gauge].current_angle = m_gauge_settings[gauge].min_angle;
	}
	else if (angle > m_gauge_settings[gauge].max_angle) {
		m_gauge_settings[gauge].current_angle = m_gauge_settings[gauge].max_angle;
	}
	else {
		m_gauge_settings[gauge].current_angle = angle;
	}

	m_cs4192.SetGaugeAngle(gauge, m_gauge_settings[gauge].current_angle + m_gauge_settings[gauge].zero_angle);
}

float AirGauge::GetPercentage(const uint8_t &gauge) const {
	// percentage = ((current_angle - min_angle) * 100) / (max_angle - min_angle)
	return ((GetAngle(gauge) - m_gauge_settings[gauge].min_angle) * 100) / (m_gauge_settings[gauge].max_angle - m_gauge_settings[gauge].min_angle);
}

void AirGauge::SetPercentage(const uint8_t &gauge, const float &percentage) {
	// current angle = (percentage * (max_angle - min_angle) / 100) + min_angle
	SetAngle(gauge, (percentage * (m_gauge_settings[gauge].max_angle - m_gauge_settings[gauge].min_angle) / 100) + m_gauge_settings[gauge].min_angle);
}

void AirGauge::Init() {
	m_cs4192.Init();


}

void AirGauge::Update() {
	m_cs4192.Update();
}

void AirGauge::DefaultGaugesConfig() {
	CalibrateGauge(Gauge::Temp, 104.16, 0, 255.84);
	CalibrateGauge(Gauge::Tacho, 104.16, 0, 255.84);
	CalibrateGauge(Gauge::Speed, 104,16, 0, 255.84);
	CalibrateGauge(Gauge::Fuel, 104.16, 0, 255.84);
}
