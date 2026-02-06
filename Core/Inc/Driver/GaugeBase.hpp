/*
 * GaugeBase.hpp
 *
 *  Created on: 6 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DRIVER_GAUGEBASE_HPP_
#define INC_DRIVER_GAUGEBASE_HPP_

#include "stdint.h"

class Gauge {
private:
	const float m_zero_angle;
	const float m_min_angle;
	const float m_max_angle;
	
	const uint16_t m_min_val;
	const uint16_t m_max_val;

	const float m_angle_per_val;

	float m_current_angle{};
public:

	Gauge(const float& zero_angle, const float& min_angle, const float& max_angle, const uint16_t& min_val, const uint16_t& max_val);

	float GetAbsoluteAngle() const;
	float GetRelativeAngle() const;
	float GetMaxAngle() const;
	float GetMinAngle() const;
	float GetZeroAngle() const;
	float GetPercentage() const;

	void SetPercentage(const float& percentage);
	void SetAngle(const float& angle);
	void SetGaugeByValue(const uint16_t& val);
};




#endif /* INC_DRIVER_GAUGEBASE_HPP_ */
