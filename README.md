
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
	my_nrf.mode = RX; 			//Mode
	my_nrf.channel = 0x20;			//RF frequency channel
	my_nrf.field_width = FW_5;		//5 bytes
	my_nrf.data_rate = DR_1;		//1 Mbps
	my_nrf.power = PW_2;			//Power of transmission
	my_nrf.data_width = 0x02;  		//payload size 1-32 bytes
	my_nrf.pipe = ALL;  			//enable all data pipe
	my_nrf.data_lenght = 2;			//2 bytes of data
	my_nrf.LNA_gain = LNA_OFF;		//LNA gain 
	my_nrf.crc_scheme = CRC_1;		//CRC
```
Notes: If you want to change the mode, you juste have to "RX" by "TX"
<br>

#### RF frequency
use a frequency of 0x20 (2,4GHz) for fiability

#### Data width
Between 1-32 bytes

#### Field width
```H
#define FW_illegal  0x00;
#define FW_3 0x01		//3 bytes
#define FW_4 0x02		//4 bytes
#define FW_5 0x03		//5bytes
```

#### Data rate
```H
#define DR_1 0x00 //1Mbps
#define DR_2 0x08 //2Mbps
```

#### Power of transmission
```H
#define PW_0 0x00  	//-18dBm
#define PW_1 0x02  	//-12dBm
#define PW_2 0x04	//-6dBm
#define PW_3 0x06	//0dBm
```

#### Init pipe
```H
#define PIP1 0x03	//enable data pipe 1
#define PIP2 0x05	//enable data pipe 2
#define PIP3 0x09	//enable data pipe 3
#define PIP4 0x11	//enable data pipe 4
#define PIP5 0x21	//enable data pipe 5

#define ALL 0x3F	//enable all data pipe 
```
PS: Data pipe 0 is use for ACK

#### LNA Gain
```H
#define LNA_ON 1
#define LNA_OFF 0
```

#### Init CRC scheme
```H
#define CRC_1 0x00	//CRC 1 byte
#define CRC_2 0x04	//CRC 2 byte
```
<br>

## Initialize your NRF24L01 and address the registers :

```C
NRF24_init(&my_nrf);		//Start intialization

	uint8_t rx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24_Set_ADDR(RX_ADDR_P0, rx_addr, 5);				//address to receive the ACK
 
 	if tx : uint8_t tx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24_Set_ADDR(TX_ADDR, tx_addr, 5);					//address to send
```

## Receive data :
```C
uint8_t rx_data[Data_lenght];
NRF24_startReceiving(&my_nrf, rx_data, 100);
```
Return : 1=Sucess and 0=Fail

## Transmit data :
```C
char data[Data_lenght] = "Your Data";
NRF24_transmit(&my_nrf, (uint8_t *) data, Data_lenght);
```
Return : 1=Sucess and 0=Fail

<br>

# Exemple of receiving data :

```C
#include "NRF24.h"

int main(){

nrf24 my_nrf;
       	my_nrf.mode = RX; 			//Mode
       	my_nrf.channel = 0x20;			//RF frequency channel
       	my_nrf.field_width = FW_5;		//5 bytes
       	my_nrf.data_rate = DR_1;		//1 Mbps
       	my_nrf.power = PW_2;			//Power of transmission
       	my_nrf.data_width = 0x02;  		//payload size 1-32 bytes
       	my_nrf.pipe = ALL;  			//enable all data pipe
       	my_nrf.data_lenght = 2;			//2 bytes of data
       	my_nrf.LNA_gain = LNA_OFF;		//Diseable LNA gain
       	my_nrf.crc_scheme = CRC_1;		//Use CRC 1 byte

      /*------------Start Init------------*/
       NRF24_init(&my_nrf);

       uint8_t rx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
       NRF24_Set_ADDR(RX_ADDR_P0, rx_addr, 5);	//address to receive

while(1){
	uint8_t rx_data[2] = "xx";
	uint8_t rx = start_Receiving(&my_nrf, rx_data, 100);
   }
}
```

# Exemple of transmitting data :

```C
#include "NRF24.h"

int main(){

nrf24 my_nrf;
       	my_nrf.mode = TX; 			//Mode
       	my_nrf.channel = 0x20;			//RF frequency channel
       	my_nrf.field_width = FW_5;		//5 bytes
       	my_nrf.data_rate = DR_1;		//1 Mbps
       	my_nrf.power = PW_2;			//Power of transmission
       	my_nrf.data_width = 0x02;  		//payload size 1-32 bytes
       	my_nrf.pipe = ALL;  			//enable all data pipe
       	my_nrf.data_lenght = 2;			//2 bytes of data
       	my_nrf.LNA_gain = LNA_OFF;		//Diseable LNA gain
       	my_nrf.crc_scheme = CRC_1;		//Use CRC 1 byte

      /*------------Start Init------------*/
       NRF24_init(&my_nrf);

       uint8_t rx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
       NRF24_Set_ADDR(RX_ADDR_P0, rx_addr, 5);	//address to receive the ACK

	uint8_t tx_addr[] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
       NRF24_Set_ADDR(TX_ADDR, tx_addr, 5);	//address to transmit

while(1){
	uint8_t tx_data[2] = "eb";
	NRF24_transmit(&my_nrf, (uint8_t *) data, 2);
   }
}
```

# Important Notes
 For a Transmitter : 
 * you have to set the same address in "TX_ADDR" and for "RX_ADDR_P0"
<br>
 
 For a Receiver :
* If you use data pipe 0 or 1, you need to define a co-errant address with your field_width that has been initialized in your config.
* If you use data pipe 2 - 5, you have to define an address of just one byte, you can use "NRF24_WriteRegister();" function.

![Addree_and_StarNetwork](https://github.com/erwannbreal/nrf24l01-Library-stm32/blob/main/Schema/Address_and_StarNetwork.png?raw=true)
![Addree](https://github.com/erwannbreal/nrf24l01-Library-stm32/blob/main/Schema/Address.png?raw=true)
