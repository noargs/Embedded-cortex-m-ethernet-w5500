#include "w5500_spi.h"


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

uint8_t __wiz5500_spi_read(void)
{
  uint8_t rb;
  rb = SPI_ReadWrite(0x00); // send dummy as we are interested only in reading
  return rb;
}

void __wiz5500_spi_write(uint8_t wb)
{
  SPI_ReadWrite(wb);
}

void __wiz5500_spi_readburst(uint8_t *buff, uint16_t len)
{
  for (uint16_t i=0; i<len; i++)
  {
	*buff = SPI_ReadWrite(0x00);
	buff++;
  }
}

void __wiz5500_spi_writeburst(uint8_t* buff, uint16_t len)
{
  for (uint16_t i=0; i<len; i++)
  {
	SPI_ReadWrite(*buff);
	buff++;
  }
}

void __wiz5500_cs_deselect(void)
{
  W5500_CS_DESELECT();
}

void __wiz5500_cs_select(void)
{
  W5500_CS_SELECT();
}

void __delay(uint8_t dur)
{
  while (dur--);
}

void W5500_Init()
{
  uint8_t memsize[2][8] = {
	  {2, 2, 2, 2, 2, 2, 2, 2},
	  {2, 2, 2, 2, 2, 2, 2, 2}
  };

  // init gpios pins `CS -> PA1` and `RST -> PC4`
  W5500_CS_RCC_CLK_ENABLE();
  W5500_CS_GPIO_INIT();

  W5500_RST_RCC_CLK_ENABLE();
  W5500_RST_GPIO_INIT();

  W5500_CS_SELECT();

  W5500_RST_PULSE_LOW();
  __delay(0xFF);
  W5500_RST_PULSE_HIGH();

  reg_wizchip_cs_cbfunc(__wiz5500_cs_select, __wiz5500_cs_deselect);
  reg_wizchip_spi_cbfunc(__wiz5500_spi_read, __wiz5500_spi_write);
  reg_wizchip_spiburst_cbfunc(__wiz5500_spi_readburst, __wiz5500_spi_writeburst);

  if (ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize) == -1)
  {
	printf("WIZCHIP Initialisation failed\r\n");
	while (1);
  }
  printf("WIZCHIP Initialised successfully...\r\n");

}
