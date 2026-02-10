/*
 * CommunicationDriver.cpp
 *
 *  Created on: 8 Feb 2026
 *      Author: Stefan
 */

#include "Driver/CommunicationDriver.hpp"
#include "Adapter/Uart_adapter.hpp"

CommunicationDriver::CommunicationDriver(UART_HandleTypeDef& huart):
m_huart{huart}
{
	auto func = Uart_adapter::DelegateFunc::create<CommunicationDriver, &CommunicationDriver::UartCallback>(*this);
	Uart_adapter::GetInstance().SetCallback(m_huart.Instance, func);
	HAL_UARTEx_ReceiveToIdle_DMA(&m_huart, m_rx_buffer.data(), m_rx_buffer.size());
}

const etl::array_view<uint8_t> CommunicationDriver::GetData() {
	m_data_ready = false;
	return etl::array_view<uint8_t>{m_data.begin(), m_data_size};
}

bool CommunicationDriver::IsDataReady() {
	return m_data_ready;
}

void CommunicationDriver::SendData(const etl::array_view<uint8_t> &data) {
	if (m_huart.gState != HAL_UART_STATE_READY) return;

	auto data_size = data.size();

	if (data_size > m_tx_buffer.size()) {
		etl::copy(c_error_msg.begin(), c_error_msg.end(), m_tx_buffer.begin());
		data_size = c_error_msg.size();
	} else {
		etl::copy(data.begin(), data.end(), m_tx_buffer.begin());
	}

	HAL_UART_Transmit_DMA(&m_huart, m_tx_buffer.data(), data_size);
}

void CommunicationDriver::UartCallback(const uint16_t &size) {
	m_copy_size = size;
	m_buffer_ready = true;
}

void CommunicationDriver::Update() {
	if (m_buffer_ready) {
		etl::copy_n(m_rx_buffer.begin(), m_copy_size, m_data.begin());
		m_data_size = m_copy_size;
		m_data_ready = true;
		m_buffer_ready = false;
	}

	HAL_UARTEx_ReceiveToIdle_DMA(&m_huart, m_rx_buffer.data(), m_rx_buffer.size());
}
