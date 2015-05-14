Sensor nodes
============

**The PROBLEM:** Problem with building cheap sensor nodes are their range and battery drain. By using each node to forward a package to the master node, (or any other node), the voltage can be scaled down so the antennas wont have the same range.

<img src="http://i.imgur.com/c1UgW4c.jpg">

The **server** is hosted on Raspberry Pi with an ultra cheap 2.4Ghz transceiver hooked-up. It captures sensor readings and also act's as an relay server for things the nodes need (like access control).

The **client** sensor nodes are atmega328 based microcontrollers that runs on batteries and have the same transceiver. They ping the server with sensor readings at given interval. They can also been given commands via RF link to set config or open doors etc.

Why? Because it's cheap.
* [$1,01 - Arduino ATmega328 3.3V](http://www.aliexpress.com/item/Free-Shipping-5PCS-LOT-Pro-Mini-Atmega328-3-3v-8MHz-For-Arduino-Compatible-nano-uno/1972594178.html)
* [$0,93 - nRF24L01](http://www.aliexpress.com/item/4pcs-NRF24L01-2-4GHz-Antenna-Wireless-Transceiver-Module-for-Arduino-New/1947680205.html)
* [$0,60 - Sensor](http://www.aliexpress.com/item/37-IN-1-BOX-SENSOR-KITS-FOR-ARDUINO-HIGH-QUALITY-FREE-SHIPPING-Works-with-Official-Arduino/1725211643.html)

So a single sensor node for $2,54 that can report measurements. Add a battery pack and more sensors, and yo're still under $10 for each node. You can get the raspberry pi for about 30-40 bucks to act as a server, but any arduino hooked to a computer will do (as well as any other microcontroller).

TODO: a lot...

**Current server output**
```
================ SPI Configuration ================
CSN Pin          = CE0 (PI Hardware Driven)
CE Pin           = Custom GPIO25
Clock Speed      = 8 Mhz
================ NRF Configuration ================
STATUS           = 0x0e RX_DR=0 TX_DS=0 MAX_RT=0 RX_P_NO=7 TX_FULL=0
RX_ADDR_P0-1     = 0xccccccccc3 0xcccccccc3c
RX_ADDR_P2-5     = 0x33 0xce 0x3e 0xe3
TX_ADDR          = 0xcccccc33e3
RX_PW_P0-6       = 0x20 0x20 0x20 0x20 0x20 0x20
EN_AA            = 0x3f
EN_RXADDR        = 0x3f
RF_CH            = 0x5a
RF_SETUP         = 0x07
CONFIG           = 0x0f
DYNPD/FEATURE    = 0x3f 0x05
Data Rate        = 1MBPS
Model            = nRF24L01+
CRC Length       = 16 bits
PA Power         = PA_MAX
starting server...
[02:25:46] [node 2] HUMID       95.000000
[02:26:35] [node 2] MOTION      1.000000
[02:26:35] [node 2] VOLTAGE     2992.000000
[02:26:38] [node 2] TEMP        1.000000
[02:26:40] [node 2] HUMID       95.000000
[02:27:35] [node 2] MOTION      1.000000
[02:27:35] [node 2] VOLTAGE     2984.000000
[02:27:36] [node 2] TEMP        8.000000
[02:27:39] [node 2] HUMID       88.000000
[02:28:35] [node 2] VOLTAGE     2984.000000
[02:28:35] [node 2] MOTION      1.000000
[02:28:35] [node 2] TEMP        31.000000
[02:28:36] [node 2] HUMID       89.000000
[02:29:35] [node 2] MOTION      1.000000
[02:29:35] [node 2] VOLTAGE     2984.000000
[02:29:36] [node 2] TEMP        36.000000
[02:29:41] [node 2] HUMID       86.000000
```
