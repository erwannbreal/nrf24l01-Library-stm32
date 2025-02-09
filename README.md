# nrf24l01-Library-stm32
Communication between two NRF24L01+ 2.4GHz chips. With STM32

# **Initialization :**
First, Include the library in your main code :

'#include "NRF24.h"'

Then, config your NRF24L01 :

'nrf24 my_nrf;
  my_nrf.mode = RX; 				//Mode
  my_nrf.channel = 0x20;			//RF frequency channel
  my_nrf.field_width = FW_5;		//5 bytes
  my_nrf.data_rate = DR_1;		//1 Mbps
  my_nrf.power = PW_2;			//Power of transmission
  my_nrf.data_width = 0x02;  		//payload size 1-32 bytes
  my_nrf.pipe = ALL;  			//enable all data pipe
  my_nrf.data_lenght = 2;			//2 bytes of data
  my_nrf.LNA_gain = LNA_OFF;
  my_nrf.crc_scheme = CRC_1;'
