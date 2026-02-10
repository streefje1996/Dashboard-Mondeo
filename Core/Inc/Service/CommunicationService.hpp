/*
 * CommunicationService.hpp
 *
 *  Created on: 10 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_SERVICE_COMMUNICATIONSERVICE_HPP_
#define INC_SERVICE_COMMUNICATIONSERVICE_HPP_

#include "Driver/CommunicationDriver.hpp"
#include "etl/string_view.h"
#include "Messages.hpp"
#include "etl/message_router.h"

class CommunicationService : public etl::message_router<CommunicationService, Log> {
private:
	InfoBus& m_info_bus;
	LogBus& m_log_bus;
	CommunicationDriver m_comm_driver;

	const GaugesInfo ExtractGaugeData(const etl::string_view& str) const;

public:
	CommunicationService(InfoBus& info_bus, LogBus& log_bus);

	void on_receive(const Log& msg);
	void on_receive_unknown(const etl::imessage& msg);

	void Update();


};



#endif /* INC_SERVICE_COMMUNICATIONSERVICE_HPP_ */
