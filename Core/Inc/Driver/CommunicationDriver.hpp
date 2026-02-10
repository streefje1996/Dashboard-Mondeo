/*
 * CommunicationDriver.hpp
 *
 *  Created on: 8 Feb 2026
 *      Author: Stefan
 */

#ifndef INC_DRIVER_COMMUNICATIONDRIVER_HPP_
#define INC_DRIVER_COMMUNICATIONDRIVER_HPP_

#include "etl/array.h"
#include "etl/array_view.h"
#include "etl/string.h"
#include "main.h"


class CommunicationDriver {
private:
	using DataArray = etl::array<uint8_t ,20>;

	inline static const etl::string<5> c_error_msg {"ERROR"};

	bool m_buffer_ready{false};
	bool m_data_ready{false};

	uint16_t m_copy_size{};
	uint16_t m_data_size{};

	DataArray m_rx_buffer{};
	DataArray m_tx_buffer{};
	DataArray m_data{};

	UART_HandleTypeDef& m_huart;


public:

	CommunicationDriver(UART_HandleTypeDef& huart);

	void UartCallback(const uint16_t& size);

	const etl::array_view<uint8_t> GetData();
	bool IsDataReady();

	void SendData(const etl::array_view<uint8_t>& data);

	void Update();
};



#endif /* INC_DRIVER_COMMUNICATIONDRIVER_HPP_ */
