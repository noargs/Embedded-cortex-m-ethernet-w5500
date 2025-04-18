<ins>Create</ins> CubeMX project `00-w5500`     
      
<ins>In CubeMx project select:</ins>     
- **Clock Configuration** tab select **HCLK** as _48MHz_ (which comes from **HSI** and **PLLCLK**)        
- **Pinout & Configuration** > **Catgories** > **SYS** > select **Debug Serial Wire** (which is connected to _PA14 SWCLK_ and _PA13 SWDIO_)      
- **Pinout & Configuration** > **Connectivity** > **SPI2** > **Mode** as _Full-Duplex Master_ and **Hardware NSS Signal** _disabled_      
     
|  Parameter Settings |   |
|:-------------|:-------------|
| Frame Format   | Motorola  |
| Data Size          | 8 Bits         |
| First Bit | MSB First         |
| Prescaler (for Baud Rate) | 2         |
| Baud Rate          | 12.0 M Bits/s         |
| Clock Polarity (CPOL)          | Low          |
| Clock Phase (CPHA)           | 1 Edge    |
| CRC Calculation           | Disabled    |        
| NSS Signal Type           | Software    |     
     

- **Pinout & Configuration** > **Connectivity** > **USART2** > **Mode** as _Asynchronous_ and **Hardware Flow Control (RS232)** _disabled_    
- **Pinout & Configuration** > **System Core** > **GPIO** > Select Pin **PA1** and **PC4** as *GPIO_Output*
- **Pinout & Configuration** > **System Core** > **RCC** > **HSE** and **LSE** are disabled and all other options are *unchecked* <ins>as we are only using HSI with **HCLK** at **48MHz**</ins>
    
|  Parameter Settings |   |
|:-------------|:-------------|
| Baud Rate   | 115200 Bits/s  |
| Word Length          | 8 Bits (including Parity)         |
| Parity | None         |
| Stop Bits | 1         |
| Data Direction          | Transmit Only         |
| Over Sampling         | 16 Samples          |    


## Things Added     
- Get the latest [I/O drivers](https://github.com/Wiznet/ioLibrary_Driver/tree/master) in **Ethernet** directory (use `socket.c/h` and `wizchip_conf.c/h` into the project in `Inc`, `Src` folder) and choose **W5500** (contain `w5500.c/h` into the project in `Inc`, `Src` folder)    
- SPI and GPIO initialization code (already done in CubeMX)   
- A function to send a byte through SPI   
- A function to send a number of bytes (array) through SPI in one go.    
- A function to receive a byte through SPI    
- A function to receive a number of bytes (array) trough SPI in one go     
- A function to control the CS IO pin   
- An over-all (full initialisation) function     
- User created files `w5500_spi.c/h` to add our code and glue with `w5500` _I/O lib_          
      

Supports 8 independent sockets simultaneously  *Datasheet Page 2*. Hence initialised 2D array in `w5500_spi.c` with 8 elements twice (For **Rx** and **Tx** Buffer with internal memory of *32Kbytes* for each buffer).     
```c
  uint8_t memsize[2][8] = {
	  {2, 2, 2, 2, 2, 2, 2, 2}, // Transmit buffer
	  {2, 2, 2, 2, 2, 2, 2, 2}  // Receive buffer
  };
```     
      
### ctlwizchip(request_code, arg)     

This function in Wizchip API have following roles.
- Reset the chip   
- Initialise the chip `ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize)`   
- Set and clear interrupt flags and enable bits    
- Get status of interrup flags and enable bits    
- Reset the PHY   
- Configure the PHY   
- Get Status of PHY link       

      		    		 