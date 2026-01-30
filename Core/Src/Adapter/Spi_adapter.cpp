/*
 * spi_adapter.cpp
 *
 *  Created on: 30 Jan 2026
 *      Author: Stefan
 */

#include <Adapter/Spi_adapter.hpp>
#include "main.h"

void Spi_adaptor::SetCallback(DelegateVoid func) {
	s_callback = func;
}

Spi_adaptor& Spi_adaptor::GetInstance() {
	static Spi_adaptor spi_adapter_instance;
	return spi_adapter_instance;
}

void Spi_adaptor::Interupt_handler() {
	s_callback.call_if();
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	Spi_adaptor::GetInstance().Interupt_handler();
}
