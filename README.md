# nrf24l01-Library-stm32
Communication between two NRF24L01+ 2.4GHz chips. With STM32

# **Requirement :**
* STM32
* NRF24L01+
<br>

![NRF24L01](https://www.atelierdelarobotique.fr/wp-content/uploads/2023/12/nrf24l01.jpg)

# **Initialization :**
First, Include the library in your main code. <br>
Download or copy [NRF24L01.c](https://github.com/erwannbreal/nrf24l01-Library-stm32/blob/main/NRF24L01.c) and [NRF24L01.h](https://github.com/erwannbreal/nrf24l01-Library-stm32/blob/main/NRF24L01.h)

```C
#include "NRF24.h"
```
<br>

Then, config your NRF24L01 :

```C
nrf24 my_nrf;
	my_nrf.mode = RX; 						//Mode
	my_nrf.channel = 0x20;				//RF frequency channel
	my_nrf.field_width = FW_5;		//5 bytes
	my_nrf.data_rate = DR_1;			//1 Mbps
	my_nrf.power = PW_2;					//Power of transmission
	my_nrf.data_width = 0x02;  		//payload size 1-32 bytes
	my_nrf.pipe = ALL;  					//enable all data pipe
	my_nrf.data_lenght = 2;				//2 bytes of data
	my_nrf.LNA_gain = LNA_OFF;		//LNA gain 
	my_nrf.crc_scheme = CRC_1;		//CRC
```
Notes: If you want to change the mode, you juste have to "RX" by "TX"

<br>
Initialize your NRF24L01 and address the registers

```C
NRF24_init(&my_nrf);																//Start intialization

	uint8_t rx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24_Set_ADDR(RX_ADDR_P0, rx_addr, 5);				//address to receive the ACK
 
 	if tx : uint8_t tx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24_Set_ADDR(TX_ADDR, tx_addr, 5);					//address to send
```

# Receive data :
```C
uint8_t rx_data[Data_lenght];
NRF24_startReceiving(&my_nrf, rx_data, 100);
```

# Transmit data :
```C
char data[Data_lenght] = "Your Data";
NRF24_transmit(&my_nrf, (uint8_t *) data, Data_lenght);
```
