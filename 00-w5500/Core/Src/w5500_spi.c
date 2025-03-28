#include "stm32f4xx_hal.h"
#include "wizchip_conf.h"
#include "stdio.h"

extern SPI_HandleTypeDef hspi2;

uint8_t SPI_ReadWrite(uint8_t data)
{
  // wait till FIFO has a free slot
  while ((hspi2.Instance->SR & SPI_FLAG_TXE) != SPI_FLAG_TXE);

  *(__IO uint8_t*)&hspi2.Instance->DR = data;

  // now wait till data arrives
  while ((hspi2.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);

  return (*(__IO uint8_t*)&hspi2.Instance->DR);
}
