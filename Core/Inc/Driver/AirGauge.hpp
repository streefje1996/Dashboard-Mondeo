/*
 * AirGauge.hpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DRIVER_AIRGAUGE_HPP_
#define INC_DRIVER_AIRGAUGE_HPP_

#include <Driver/GaugeBase.hpp>
#include "Device/CS4192.hpp"
#include <etl/array.h>

#define TEST_SETUP

#ifdef TEST_SETUP
enum Gauges { TACHO, SPEED, TEMP, FUEL};
#else
enum Gauges { TEMP, TACHO, SPEED, FUEL};
#endif

class AirGauge {
public:
	AirGauge();

	void SetGaugeByValue(const Gauges& gauge, const uint16_t& val);

	void Init();
	void Update();
	void Shutdown();

	void ShutdownCallback();
	void StartUpCallback();

private:
	static constexpr uint8_t s_num_gauges{4};

	CS4192<s_num_gauges> m_cs4192;

	etl::array<Gauge, s_num_gauges> m_gauges;

	bool MoveGaugeToUpperLimit(const bool& max);
};

#endif /* INC_DRIVER_AIRGAUGE_HPP_ */
