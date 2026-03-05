/*
 * UIService.cpp
 *
 *  Created on: 6 Feb 2026
 *      Author: Stefan
 */

#include "Service/UIService.hpp"

UIService::UIService(LogBus& log_bus, ScreenInterface& screen) :
m_log_bus{log_bus}, m_screen{screen}
{

}

void UIService::on_receive(const Start &msg) {
	m_air_gauge.Init();
	m_lights.Init();
	m_screen.EnableDisplay();
}

void UIService::on_receive(const Stop &msg) {
	m_air_gauge.Shutdown();
}

void UIService::on_receive(const GaugesInfo &msg) {
	m_air_gauge.SetGaugeByValue(Gauges::SPEED, msg.speed);
	m_air_gauge.SetGaugeByValue(Gauges::TACHO, msg.rpm);
	m_air_gauge.SetGaugeByValue(Gauges::FUEL, msg.fuel);
	m_air_gauge.SetGaugeByValue(Gauges::TEMP, msg.temp);
	m_air_gauge.Update();
}

void UIService::on_receive(const LightInfo &msg) {
	m_lights.SetLight(msg.light, msg.color);

}

void UIService::on_receive(const ScreenInfo &msg) {
	m_screen.SetString(0, 30, msg.message);
}

void UIService::Update() {
	m_lights.Update();
	m_screen.Update();
}

void UIService::on_receive_unknown(const etl::imessage &msg) {
	m_log_bus.receive(Log{"Unknown Message"});
}
