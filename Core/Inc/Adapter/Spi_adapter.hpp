/*
 * spi_adapter.hpp
 *
 *  Created on: 30 Jan 2026
 *      Author: Stefan
 */

#ifndef INC_ADAPTER_SPI_ADAPTER_HPP_
#define INC_ADAPTER_SPI_ADAPTER_HPP_

#include <etl/delegate.h>

class Spi_adaptor {
public:
	using DelegateVoid = etl::delegate<void(void)>;

	static Spi_adaptor& GetInstance();

	void SetCallback(DelegateVoid func);
	static void Interupt_handler();

private:
	static inline DelegateVoid s_callback;

	Spi_adaptor() = default;
	~Spi_adaptor() = default;

	Spi_adaptor(Spi_adaptor&) = delete;
	void operator=(const Spi_adaptor&) = delete;
};

#endif /* INC_ADAPTER_SPI_ADAPTER_HPP_ */
