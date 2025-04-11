<ins>Create</ins> CubeMX project `04-mqtt-publisher`        

- STM32+W5500 act as a MQTT Client. PC as a client as well as server. And android phone as client.    
- Get the [Eclipse Mosquitto](https://mosquitto.org/download/), a MQTT Server for PC. Install and go to the directory. Create a file `myconfig.conf` with following content. (This process needed as to expose it not only locally but to the whole internet)
```bash
listener 1883
allow_anonymous true
```     
- Run MQTT server as follows:    
```bash
C:\Program Files\mosquitto> mosquitto -v -c myconfig.conf

# if port 1883 for MQTT is already in use then kill the process as follows   
# find the port use by process 
$ netstat -ano | findstr 1883 

# kill the process using this process id
$ taskkill /F /PID <Process-ID i.e. 16528>  

# Start the MQTT server again with `myconfig.conf` file
C:\Program Files\mosquitto> mosquitto -v -c myconfig.conf
```     
- Download/Install MQTT Client, [MQTTX](https://mqttx.app/downloads), for PC. Which helps us view the message sent by STM32 Client and we can also send the messages back to STM32 Client.   
- Install MQTT Client on Android by going to Playstore and searching [MyMQTT](https://play.google.com/store/apps/details?id=at.tripwire.mqtt.client).   

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
      
### Integrate Eclipse Paho to CubeMX project     
- Get the latest Eclipse Paho [code](https://github.com/eclipse-paho/paho.mqtt.embedded-c) from github.    
- Move `MQTTClient.c/h` files from **MQTC** directory and `mqtt_interface.c/h` files from **MQTTPacket** directory, into the IDE.    
- Also add headers to the Include path in the compiler by going to the **Properties** of the project and then **C/C++ Build** > **Settings** > **MCU/MPU GCC Compiler** > **Include Paths** as `../Core/Inc/mqttc`        


> Allot the Static IP to W5500 in `wiz_netinfo` struct (in `main.c`) by making sure not already alloted by the Router to other device. IPs over xxx.xxx.xxx.100 are generally safe to use. Gateway IP can be obtained on the window by running `ipconfig` in the terminal (Not usefull for local TCP application). 

> Destination IP `server_ip[]` (in `main.c`) is your PC IP which can be obtained by running `ipconfig` in the terminal. We will create a UDP Client by installing [Hercules](https://www.hw-group.com/software/hercules-setup-utility), giving ip `127.0.01` and selecting a `5000` port.

> TCP Port `5000` is used which has to be opened from Windows Firewall by going to **Control Panel** > **Windows Defender Firewall** > **Advanced settings** clicking on the left tray on **Inbound Rules** and click on right tray on **New Rule** Selecting **Port** and click <ins>Next</ins>. Select **TCP** and enter 5000 in **Specific local ports** text field, click <ins>Next</ins>. Now, Pick **Allow the connection** click <ins>Next</ins>. Next, pick all three check boxes **Domain**, **Private** and **Public** and click <ins>Next</ins>. Finally give the name (i.e. `Open Port W5500`) to this port so you can identify/search later and click <ins>**Finish**</ins>.