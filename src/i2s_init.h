#ifndef _I2C_INIT_H
#define _I2C_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
extern I2S_HandleTypeDef hi2s2;
extern I2S_HandleTypeDef hi2s1;

void MX_GPIO_Init_I2S1(void);
void MX_GPIO_Init_I2S2(void);
void MX_I2S1_Init(unsigned long audioFreq);
void MX_I2S2_Init(unsigned long audioFreq);
void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s);

#ifdef __cplusplus
}
#endif

#endif