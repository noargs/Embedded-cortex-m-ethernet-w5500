<ins>Create</ins> CubeMX project `02-w5500-udp-server`        

- Our PC is UDP Server (UDP Server is created by [Herculas](https://www.hw-group.com/software/hercules-setup-utility))
- STM32+W5500 as UDP Client
      
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
     
          
## UDP    
- Ureliable, Datagram (packets) oriented as opposed to TCP which is Stream oriented.     
- Connectionless, and <ins>doesnt</ins> have `connect()` and `disconnect()` APIs. Only `socket()`, `sendto()`, `recvfrom()` and `close()` APIs.    
- Packets are received not in order hence you can make field in packet with a serial number to figure out the correct order at receiver side.    
- Speed is crucial and some data loss is acceptable    
- General applications are Live audio/video and other realtime application     

## sendto()     
```c
sendto(
  sn, // socket number
  buff, // pointer to the data wish to send
  len,  // length of the data
  addr, // 4 byte IP address of the destination host (server)
  port  // port number of the destination host (server)
)
```        
- Unlike TCP, Single UDP Socket can be used to send data to completely two different hosts. Also different ports on same host.    
> <ins>Maximum size</ins> of a datagram is less than 500 bytes for Internet *OR* 1000 bytes in Local ethernet application. If you have payload of 2000 bytes then split it 500 bytes of 4 datagrams


## Client send message to UDP server every 1 sec   

- A client (STM32+W5500) will send the message to UDP server (running Herculas in our PC, using `127.0.0.1` ip and `5000` port)    
    


> Allot the Static IP to W5500 in `wiz_netinfo` struct (in `main.c`) by making sure not already alloted by the Router to other device. IPs over xxx.xxx.xxx.100 are generally safe to use. Gateway IP can be obtained on the window by running `ipconfig` in the terminal (Not usefull for local TCP application). 

> Destination IP `server_ip[]` (in `main.c`) is your PC IP which can be obtained by running `ipconfig` in the terminal. We will create a UDP Client by installing [Hercules](https://www.hw-group.com/software/hercules-setup-utility), giving ip `127.0.01` and selecting a `5000` port.

> TCP Port `5000` is used which has to be opened from Windows Firewall by going to **Control Panel** > **Windows Defender Firewall** > **Advanced settings** clicking on the left tray on **Inbound Rules** and click on right tray on **New Rule** Selecting **Port** and click <ins>Next</ins>. Select **TCP** and enter 5000 in **Specific local ports** text field, click <ins>Next</ins>. Now, Pick **Allow the connection** click <ins>Next</ins>. Next, pick all three check boxes **Domain**, **Private** and **Public** and click <ins>Next</ins>. Finally give the name (i.e. `Open Port W5500`) to this port so you can identify/search later and click <ins>**Finish**</ins>.