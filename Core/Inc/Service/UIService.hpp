/*
 * UIService.hpp
 *
 *  Created on: 5 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_SERVICE_UISERVICE_HPP_
#define INC_SERVICE_UISERVICE_HPP_

#include "Messages.hpp"
#include <etl/message_router.h>
#include "Driver/AirGauge.hpp"

class UIService : public etl::message_router<UIService, Start, Stop, GaugesInfo, LightInfo, ScreenInfo> {
private:
	LogBus& m_log_bus;
	AirGauge m_air_gauge;

public:
	UIService(LogBus& log_bus);

	void on_receive(const Start& 		msg);
	void on_receive(const Stop& 		msg);
	void on_receive(const GaugesInfo& 	msg);
	void on_receive(const LightInfo& 	msg);
	void on_receive(const ScreenInfo& 	msg);
	void on_receive_unknown(const etl::imessage& msg);
};




#endif /* INC_SERVICE_UISERVICE_HPP_ */
