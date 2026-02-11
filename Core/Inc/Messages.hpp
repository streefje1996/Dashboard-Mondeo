/*
 * Messages.hpp
 *
 *  Created on: 5 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_MESSAGES_HPP_
#define INC_MESSAGES_HPP_

#include <etl/message.h>
#include <etl/message_bus.h>
#include <etl/string.h>
#include "Lights.hpp"

#define MAX_STRING_LENGTH 20

using InfoBus = etl::message_bus<1>;
using LogBus = etl::message_bus<1>;

enum MsgId {
	START,
	STOP,
	GAUGES_INFO,
	LIGHT_INFO,
	SCREEN_INFO,
	LOG
};

struct Start : public etl::message<MsgId::START> {

};

struct Stop : public etl::message<MsgId::STOP> {

};

struct GaugesInfo : public etl::message<MsgId::GAUGES_INFO> {
	uint16_t speed{};
	uint16_t rpm{};
	uint8_t fuel{};
	uint8_t temp{};
};


struct LightInfo : public etl::message<MsgId::LIGHT_INFO> {
	Lights light{};
	Color color{Color::WHITE};
};

struct ScreenInfo : public etl::message<MsgId::SCREEN_INFO> {
	etl::string<MAX_STRING_LENGTH> message{};
};


struct Log : public etl::message<MsgId::LOG> {
	etl::string<MAX_STRING_LENGTH> message;

	Log(const etl::string<MAX_STRING_LENGTH>& msg) : message{msg} {}
};

#endif /* INC_MESSAGES_HPP_ */
