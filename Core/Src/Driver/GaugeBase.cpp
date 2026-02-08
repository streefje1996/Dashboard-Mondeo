/*
 * GaugeBase.cpp
 *
 *  Created on: 6 Feb 2026
 *      Author: Stefan
 */

#include "Driver/GaugeBase.hpp"

Gauge::Gauge(const float& zero_angle, const float& min_angle, const float& max_angle, const uint16_t& min_val, const uint16_t& max_val) :
m_zero_angle{zero_angle}, m_min_angle{min_angle}, m_max_angle{max_angle}, m_min_val{min_val}, m_max_val{max_val},
m_angle_per_val{(max_angle - min_angle) / (max_val - min_val)}
{
}

float Gauge::GetAbsoluteAngle() const {
	return m_current_angle;
}

float Gauge::GetRelativeAngle() const {
	return m_current_angle + m_zero_angle;
}

float Gauge::GetMaxAngle() const {
	return m_max_angle;
}

float Gauge::GetMinAngle() const {
	return m_min_angle;
}

float Gauge::GetZeroAngle() const {
	return m_zero_angle;
}

void Gauge::SetAngle(const float& angle) {
	if (angle < m_min_angle) {
		m_current_angle = m_min_angle;
	}
	else if (angle > m_max_angle) {
		m_current_angle = m_max_angle;
	}
	else {
		m_current_angle = angle;
	}
}

void Gauge::SetPercentage(const float &percentage) {
	SetAngle((percentage * (m_max_angle - m_min_angle) / 100) + m_min_angle);
}

float Gauge::GetPercentage() const {
	return ((GetAbsoluteAngle() - m_min_angle) * 100) / (m_max_angle - m_min_angle);
}

void Gauge::SetGaugeByValue(const uint16_t& val) {
	uint16_t val_limited = val;
	if (val < m_max_val) {
		val_limited = m_max_val;
	}
	else if (val > m_min_val) {
		val_limited = m_min_val;
	}
	else {
		val_limited = val;
	}

	SetAngle(val_limited * m_angle_per_val);
}
