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

const GaugesInfo CommunicationService::ExtractGaugeData(const etl::string_view &str) const {

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

const LightInfo CommunicationService::ExtractLightData(const etl::string_view &str) const {

	LightInfo light_info{};

	uint8_t size = str[1] > '9' ? 1 : 2;
	uint8_t light_id = etl::to_arithmetic<uint8_t>(str.substr(0, size));

	light_info.light = Lights{light_id};

	switch (str[str.size()-1]) {
		case 'R':
			light_info.color = Color::RED;
			break;
		case 'G':
			light_info.color = Color::GREEN;
			break;
		case 'B':
			light_info.color = Color::BLUE;
			break;
		case 'W':
			light_info.color = Color::WHITE;
			break;
		case 'O':
			light_info.color = Color::ORANGE;
			break;
		case 'F':
			light_info.color = Color::OFF;
			break;
		default:
			light_info.color = Color::WHITE;
			break;
	}

	return light_info;
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

		uint8_t search_pos{};
		int8_t size{};

		while (search_pos != str.length()) {
			size = str.find(';', search_pos);
			if (size == -1) break;
			size -= search_pos;

			auto sub_str = str.substr(search_pos, size);

			search_pos += size + 1;

			switch (sub_str[0]) {
				case 'G': // gauges
					m_info_bus.receive(ExtractGaugeData(sub_str.substr(1, sub_str.size()-1)));
					break;
				case 'L': // lights
					m_info_bus.receive(ExtractLightData(sub_str.substr(1, sub_str.size()-1)));
					break;
				case 's': // start
					m_info_bus.receive(Start{});
					break;
				case 'q': // quit
					m_info_bus.receive(Stop{});
					break;
				default:
					break;
			}
		}

	}
}

