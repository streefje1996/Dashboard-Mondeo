/*
 * CommunicationService.cpp
 *
 *  Created on: 10 Feb 2026
 *      Author: Stefan
 */

#include "Service/CommunicationService.hpp"
#include "usart.h"
#include "etl/to_arithmetic.h"
#include "etl/array.h"


CommunicationService::CommunicationService(InfoBus &info_bus, LogBus& log_bus) :
m_info_bus{info_bus}, m_log_bus{log_bus}, m_comm_driver{huart2}
{
}

const GaugesInfo CommunicationService::ExtractGaugeData(
		const etl::string_view &str) const {

	GaugesInfo gauge_info{};
	etl::array<uint16_t, 4> gauge_buffer;
	etl::string<6> sub_str{};
	uint8_t index{};


	for (const auto& c : str) {
		if(c==',') {
			gauge_buffer[index++] = etl::to_arithmetic<uint16_t>(sub_str);
			sub_str.clear();
			continue;
		}

		sub_str.push_back(c);
	}

	gauge_info.temp = gauge_buffer[0];
	gauge_info.rpm = gauge_buffer[1];
	gauge_info.speed = gauge_buffer[2];
	gauge_info.fuel = gauge_buffer[3];

	return gauge_info;
}


void CommunicationService::on_receive(const Log &msg) {
	m_comm_driver.SendData(etl::array_view<uint8_t>{(uint8_t*)(msg.message.data()), msg.message.size()});
}

void CommunicationService::on_receive_unknown(const etl::imessage &msg) {
	m_log_bus.receive(Log{"Unknown Message"});
}

void CommunicationService::Update() {
	m_comm_driver.Update();

	if (m_comm_driver.IsDataReady()) {
		auto data = m_comm_driver.GetData();
		etl::string_view str{(char*)data.data(), data.size()};
		switch (str[0]) {
			case 'G': // gauges
				m_info_bus.receive(ExtractGaugeData(str.substr(1, str.size()-1)));
				break;
			case 's': // start
				m_info_bus.receive(Start{});
				break;
			default:
				break;
		}
	}
}
