/*
 * I2c_adapter.hpp
 *
 *  Created on: 5 Mar 2026
 *      Author: Stefan
 */

#ifndef INC_ADAPTER_I2C_ADAPTER_HPP_
#define INC_ADAPTER_I2C_ADAPTER_HPP_

#include <etl/delegate.h>
#include <etl/flat_map.h>
#include "main.h"

class I2c_adapter {
public:
	using DelegateVoid = etl::delegate<void(void)>;
	using DelegateMap = etl::flat_map<I2C_TypeDef*, DelegateVoid, 1>;

	static I2c_adapter& GetInstance();

	void SetDmaCompletedCallback(I2C_TypeDef* ic2, const DelegateVoid& func);
	static void DmaCompleteInteruptHandler(I2C_TypeDef* i2c);

private:
	static inline DelegateMap s_dma_completed_callback;


	I2c_adapter() = default;
	~I2c_adapter() = default;

	I2c_adapter(I2c_adapter&) = delete;
	void operator=(const I2c_adapter&) = delete;
};



#endif /* INC_ADAPTER_I2C_ADAPTER_HPP_ */
