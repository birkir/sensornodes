Sensor nodes
============

The server is hosted on Raspberry Pi with an ultra cheap 2.4Ghz transceiver hooked-up. It captures sensor readings and also act's as an relay server for things the nodes need (like access control).

The sensor nodes are atmega328 based microcontrollers that runs on batteries and have the same transceiver. They ping the server with sensor readings at given interval. They can also been given commands via RF link to set config or open doors etc.

TODO: a lot...

**Current output**
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
