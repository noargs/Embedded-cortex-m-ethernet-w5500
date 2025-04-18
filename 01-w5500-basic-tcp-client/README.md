<ins>Create</ins> CubeMX project `01-w5500-basic-tcp-client`        

- Our PC is TCP Server (TCP Server is created by [Herculas](https://www.hw-group.com/software/hercules-setup-utility))
- STM32+W5500 is client
      
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
      
### Integrate Wiznet W5500 Lib to CubeMX project   
- Get the latest [I/O drivers](https://github.com/Wiznet/ioLibrary_Driver/tree/master) in **Ethernet** directory (use `socket.c/h` and `wizchip_conf.c/h` into the project in `Inc`, `Src` folder) and choose **W5500** (contain `w5500.c/h` into the project in `Inc`, `Src` folder).   
- Also add headers to the Include path in the compiler by going to the **Properties** of the project and then **C/C++ Build** > **Settings** > **MCU/MPU GCC Compiler** > **Include Paths** as `../Core/Inc/w5500`     			


## Static Host configuration    
- Before a device (i.e. W5500) on IP network can participate in the data transfer, it must be configured.   
- It needs some data about the network, to become a part of that network.   
- And that data is as follows:   
-- Its own address that is IP address (for local netwrork)  
-- Subnet mask of the network (for local network)  
-- IP address of the gateway in that network (for internet)  
-- IP address of the domain name server that is DNS (for internet)    
     
<ins>How the Device gets all the above information</ins>. **Host configuration** can be done either by **Static** or **Dynamic** way.     
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
     
### Thing we need    
- Internet Router, it has several LAN ports, WAN port (Internet) and WiFi connectivity. It also performs the role of a *gateway*.    
- A LAN cable to connect our W5500 to this network.  
- The Embedded Host, that is our STM32+W5500     

### Disable the buffering of STDOUT    
As `stdout` is buffered to the memory first and the output is not immediately transfered to the device (USART). we will disable it as follows  
```c
#include <stdio.h>

int main(void)
{
	setbuf(stdout, NULL);
}
```

> Allot the Static IP to W5500 in `wiz_netinfo` struct (in `main.c`) by making sure not already alloted by the Router to other device. IPs over xxx.xxx.xxx.100 are generally safe to use. Gateway IP can be obtained on the window by running `ipconfig` in the terminal (Not usefull for local TCP application). 

> Destination IP `dest_ip[]` (in `main.c`) is your PC IP which can be obtained by running `ipconfig` in the terminal. We will create a TCP Server by installing [Hercules](https://www.hw-group.com/software/hercules-setup-utility) and select a `5000` port.

> TCP Port `5000` is used which has to be opened from Windows Firewall by going to **Control Panel** > **Windows Defender Firewall** > **Advanced settings** clicking on the left tray on **Inbound Rules** and click on right tray on **New Rule** Selecting **Port** and click <ins>Next</ins>. Select **TCP** and enter 5000 in **Specific local ports** text field, click <ins>Next</ins>. Now, Pick **Allow the connection** click <ins>Next</ins>. Next, pick all three check boxes **Domain**, **Private** and **Public** and click <ins>Next</ins>. Finally give the name (i.e. `Open Port W5500`) to this port so you can identify/search later and click <ins>**Finish**</ins>.