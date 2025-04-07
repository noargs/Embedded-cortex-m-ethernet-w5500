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


## Static Host configuration    
- Before a host on IP network can participate in the data transfer, it must be configured.   
- It needs some data about the network, to become a part of that network.   
- And that data is as follows:   
-- Its own address that is IP address (for local netwrork)  
-- Subnet mask of the network (for local network)  
-- IP address of the gateway in that network (for internet)  
-- IP address of the domain name server that is DNS (for internet)    
     
<ins>How the Host gets all the above information</ins>. **Host configuration** can be done either by **Static** or **Dynamic** way.     
We will demonstrate **Static** method as it is simple and easy to implement in code for now.     
```c
wiz_NetInfo wiz_netinfo = {
	.mac 	= {0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef},
	.ip 	= {192, 168, 1, 112},
	.sn		= {255, 255, 255, 0},
	.gw		= {192, 168, 1, 1},
	.dns	= {8, 8, 8, 8},
	.dhcp	= {NETINFO_STATIC}
}; 

int main()
{
  ...

  ctlnetwork(CN_SET_NETINFO, (void*)&wiz_netinfo);

  ...
}
```     
     
### Thing we will be needed    
- Internet Router, it has several LAN ports, WAN port (Internet) and WiFi connectivity. It also performs the role of a *gateway*.    
- A LAN cable to connect our W5500 to this network.  
- The Embedded Host, that is our STM32+W5500     

      		    		 