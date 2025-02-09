/*
 * NRF24.h
 *
 *  Created on: Jan 19, 2025
 *  Author: Erwann BREAL
 *  github : https://github.com/erwannbreal
 *
 */

#ifndef INC_NRF24_H_
#define INC_NRF24_H_

#include "main.h"

//-----------------Config-----------------------

#define nrf24_spi &hspi1				//SPI handle

#define CE_GPIO_Port	GPIOB
#define CE_Pin			GPIO_PIN_0

#define CSN_GPIO_Port	GPIOB
#define CSN_Pin			GPIO_PIN_1

//----------------------------------------------

//-------field Width--------
#define FW_illegal  0x00;
#define FW_3 0x01
#define FW_4 0x02
#define FW_5 0x03
//--------------------------

//--------Data Rate---------
#define DR_1 0x00 //1Mbps
#define DR_2 0x08 //2Mbps
//--------------------------

//---------Power------------
#define PW_0 0x00  //-18dBm
#define PW_1 0x02  //-12dBm
#define PW_2 0x04	//-6dBm
#define PW_3 0x06	//0dBm
//--------------------------

//----------Pipe------------
#define PIP1 0x03
#define PIP2 0x05
#define PIP3 0x09
#define PIP4 0x11
#define PIP5 0x21

#define ALL 0x3F
//--------------------------

//-------Mode---------------
#define RX 1
#define TX 0
//--------------------------

//-------LNA gain-----------
#define LNA_ON 1
#define LNA_OFF 0
//--------------------------

//-------CRC Scheme---------
#define CRC_1 0x00		//CRC 1 byte
#define CRC_2 0x04		//CRC 2 byte
//--------------------------

typedef struct nrf24_config{
	uint8_t mode;				//Rx/Tx	Mode
	uint8_t channel;			//Frequency channel
	uint8_t field_width;
	uint8_t data_rate;
	uint8_t power;				//Power of transmission
	uint8_t data_width;
	uint8_t pipe;				//data pipe that we use
	uint8_t data_lenght;		//lenght of data
	uint32_t rx_address;
	uint32_t tx_address;
	uint8_t LNA_gain;
	uint8_t crc_scheme;
} nrf24;

//---------Registers--------
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define OBSERVE_TX 0x08
#define CD 0x09
#define TX_ADDR 0x10
#define RX_PW_P0 0x11
#define RX_PW_P1 0x12
#define	RX_PW_P2 0x13
#define RX_PW_P3 0x14
#define RX_PW_P4 0x15
#define RX_PW_P5 0x16
#define FIFO_STATUS 0x17

#define RX_ADDR_P0 0x0A //5 bytes register
#define RX_ADDR_P1 0x0B //5 bytes register
#define RX_ADDR_P2 0x0C //1 bytes register
#define RX_ADDR_P3 0x0D //1 bytes register
#define RX_ADDR_P4 0x0E //1 bytes register
#define RX_ADDR_P5 0x0F //1 bytes register

#define FEATURE 0x1D
//--------------------------

//---------------Commands-------------
#define CMD_FLUSH_TX 		0xE1   		//Reset TX-fifo
#define CMD_FLUSH_RX 		0xE2		//Reset RX-fifo
#define CMD_WRITE_TX 		0xA0		//Send Tx payload
#define CMD_READ_RX 		0x61		//Send RX payload
#define CMD_REFUSE_PAYLOAD 	0xE3 		//Refuse to receive Tx-payload
//-------------------------------------

//function to add : switch mode (RX<->TX) and switch mode (PWR_UP<->PWR_DW)
//ajouter le choix sur la fifo que l'on souhaite utiliser dans la config main
uint8_t NRF24_init(nrf24* _nrf24);
uint8_t NRF24_SendCommand(uint8_t cmd);
uint8_t NRF24_WriteRegister(uint8_t reg, uint8_t value);
uint8_t NRF24_ReadRegister(uint8_t reg);
uint8_t NRF24_startReceiving(nrf24* _nrf24, uint8_t* data_buf, uint8_t timeout);
uint8_t NRF24_transmit(nrf24* _nrf24, uint8_t *data, uint8_t lenght);
void flush_TX(void);
void flush_RX(void);
uint8_t set_LSB(uint8_t data);
uint8_t NRF24_Set_ADDR(uint8_t reg, uint8_t *addr, uint8_t lenght);
void check_registers(void);

#endif /* INC_NRF24_H_ */
