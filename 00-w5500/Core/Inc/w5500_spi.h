#ifndef INC_W5500_SPI_H_
#define INC_W5500_SPI_H_

#include "stm32f4xx_hal.h"
#include "wizchip_conf.h"
#include "stdio.h"

#define W5500_CS_PIN				GPIO_PIN_1
#define W5500_CS_PORT				GPIOA

#define W5500_CS_SELECT()			\
		  HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_RESET)

#define W5500_CS_DESELECT()			\
		  HAL_GPIO_WritePin(W5500_CS_PORT, W5500_CS_PIN, GPIO_PIN_SET)

#define W5500_CS_RCC_CLK_ENABLE()	\
		  __HAL_RCC_GPIOA_CLK_ENABLE()

#define W5500_CS_GPIO_INIT()		\
		  GPIO_InitTypeDef GPIO_CSInitStruct = {0};			\
		  GPIO_CSInitStruct.Pin 	= W5500_CS_PIN;			\
		  GPIO_CSInitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;	\
		  GPIO_CSInitStruct.Pull 	= GPIO_NOPULL;			\
		  GPIO_CSInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	\
		  HAL_GPIO_Init(W5500_CS_PORT, &GPIO_CSInitStruct)

#define W5500_RST_PIN				GPIO_PIN_4
#define W5500_RST_PORT				GPIOC

#define W5500_RST_RCC_CLK_ENABLE()  \
		  __HAL_RCC_GPIOC_CLK_ENABLE()

#define W5500_RST_GPIO_INIT()		\
		  GPIO_InitTypeDef GPIO_RSTInitStruct = {0};		\
		  GPIO_RSTInitStruct.Pin 	= W5500_RST_PIN;		\
		  GPIO_RSTInitStruct.Mode 	= GPIO_MODE_OUTPUT_PP;	\
		  GPIO_RSTInitStruct.Pull 	= GPIO_NOPULL;			\
		  GPIO_RSTInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;	\
		  HAL_GPIO_Init(W5500_RST_PORT, &GPIO_RSTInitStruct)

#define W5500_RST_PULSE_HIGH()		\
		  HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_SET)

#define W5500_RST_PULSE_LOW()		\
		  HAL_GPIO_WritePin(W5500_RST_PORT, W5500_RST_PIN, GPIO_PIN_RESET)


void	W5500_Init();


#endif /* INC_W5500_SPI_H_ */
