/*
 * I2c_adapter.cpp
 *
 *  Created on: 5 Mar 2026
 *      Author: Stefan
 */

#include "Adapter/I2c_adapter.hpp"

I2c_adapter& I2c_adapter::GetInstance() {
	static I2c_adapter s_i2c_adapter;
	return s_i2c_adapter;
}

void I2c_adapter::SetDmaCompletedCallback(I2C_TypeDef* ic2, const DelegateVoid& func) {
	s_dma_completed_callback[ic2] = func;
}

void I2c_adapter::DmaCompleteInteruptHandler(I2C_TypeDef* ic2) {
	s_dma_completed_callback[ic2].call_if();
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
	I2c_adapter::DmaCompleteInteruptHandler(hi2c->Instance);
}


