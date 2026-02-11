/*
 * AirGauge.cpp
 *
 *  Created on: 3 Feb 2026
 *      Author: Stefan
 */

#include "Driver/AirGauge.hpp"
#include "Adapter/Timer_adapter.hpp"
#include "tim.h"

AirGauge::AirGauge():
m_cs4192{hspi1, SPI1_CS_GPIO_Port, SPI1_CS_Pin, ACG_OE_GPIO_Port, ACG_OE_Pin},
#ifdef TEST_SETUP
m_gauges{
	Gauge{104.16, 0, 255.84, 0, 8000}, //TACHO
	Gauge{104.16, 0, 255.84, 0, 240}, //SPEED
	Gauge{104.16, 0, 255.84, 60, 120}, //TEMP
	Gauge{104.16, 0, 255.84, 0, 100} //FUEL
}
#else
m_gauges{
	Gauge{104.16, 0, 255.84, 60, 120}, //TEMP
	Gauge{104.16, 0, 255.84, 0, 8000}, //TACHO
	Gauge{104.16, 0, 255.84, 0, 240}, //SPEED
	Gauge{104.16, 0, 255.84, 0, 100} //FUEL
}
#endif
{
}

void AirGauge::SetGaugeByValue(const Gauges& gauge, const uint16_t& val) {
	m_gauges[gauge].SetGaugeByValue(val);

	m_cs4192.SetGaugeAngle(gauge, m_gauges[gauge].GetRelativeAngle());
}

void AirGauge::Init() {
	m_cs4192.Init();
	Timer_adapter::GetInstance().SetCallback(htim2.Instance, etl::delegate<void(void)>::create<AirGauge, &AirGauge::StartUpCallback>(*this));
	HAL_TIM_Base_Start_IT(&htim2);
}

void AirGauge::Update() {
	m_cs4192.Update();
}

void AirGauge::Shutdown() {
	Timer_adapter::GetInstance().SetCallback(htim2.Instance, etl::delegate<void(void)>::create<AirGauge, &AirGauge::ShutdownCallback>(*this));
	HAL_TIM_Base_Start_IT(&htim2);
}

void AirGauge::ShutdownCallback() {
	if (!MoveGaugeToUpperLimit(false)) return;
	HAL_TIM_Base_Stop_IT(&htim2);
}

void AirGauge::StartUpCallback() {
	if (!MoveGaugeToUpperLimit(true)) return;
	HAL_TIM_Base_Stop_IT(&htim2);
	Shutdown();
}

// TODO: Need a better function for this, will do for now.
bool AirGauge::MoveGaugeToUpperLimit(const bool& max) {
	float total{};
	float target_total{};
	uint8_t i = 0;

	for (auto& gauge : m_gauges) {
		if (max) {
			gauge.SetPercentage(gauge.GetPercentage() + 1.f);
			target_total+=gauge.GetMaxAngle();
		} else {
			gauge.SetPercentage(gauge.GetPercentage() - 1.f);
			target_total+=gauge.GetMinAngle();
		}
		m_cs4192.SetGaugeAngle(i++, gauge.GetRelativeAngle());
		total+=gauge.GetAbsoluteAngle();
	}
	Update();

	if (total == target_total) {
		return true;
	}

	return false;
}
