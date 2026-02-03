/*
 * AirGauge.hpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DRIVER_AIRGAUGE_HPP_
#define INC_DRIVER_AIRGAUGE_HPP_

#include "Device/CS4192.hpp"
#include <etl/array.h>

enum Gauge {Temp, Tacho, Speed, Fuel};

struct GaugeSettings {
	float zero_angle{};
	float min_angle{};
	float max_angle{};
	float current_angle{};
};

class AirGauge {
public:

	AirGauge();

	void CalibrateGauge(const uint8_t& gauge, const float& zero_angle, const float& min_angle, const float& max_angle);

	float GetAngle(const uint8_t& gauge) const;
	void SetAngle(const uint8_t& gauge, const float& angle);

	float GetPercentage(const uint8_t& gauge) const;
	void SetPercentage(const uint8_t& gauge, const float& percentage);

	void Init();
	void Update();
	void Shutdown();

	void ShutdownCallback();
	void StartUpCallback();

private:
	static constexpr uint8_t s_num_gauges{4};

	CS4192<s_num_gauges> m_cs4192;

	etl::array<GaugeSettings, s_num_gauges> m_gauge_settings;

	void DefaultGaugesConfig();

};

#endif /* INC_DRIVER_AIRGAUGE_HPP_ */
