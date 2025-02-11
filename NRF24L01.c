/*
 * NRF24.c
 *
 *  Created on: Jan 13, 2025
 *  Author: Erwann BREAL
 *  github : https://github.com/erwannbreal
 *
 */
#include "main.h"
#include "NRF24.h"

extern SPI_HandleTypeDef hspi1;

/*
 * Function : Init NRF24L01
 * Return : 1=init complete, 0=init fail
 * parameters : nrf24 config
 */
uint8_t NRF24_init(nrf24* _nrf24){
	if(_nrf24->mode == RX){
		HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET);			//Standby Mode

		uint8_t config = 0x0B | _nrf24->crc_scheme;
		NRF24_WriteRegister(CONFIG, config); 					//Config -> 0111001 = CRC 1 byte + RX

		NRF24_WriteRegister(EN_RXADDR, _nrf24->pipe); 			//enable data pipe
		NRF24_WriteRegister(EN_AA, _nrf24->pipe); 				//enable ack for data pipe
		NRF24_WriteRegister(SETUP_AW, _nrf24->field_width); 	//largeur de champ d'adresse
		NRF24_WriteRegister(SETUP_RETR, 0x4F); 					//auto acknowledgement=ON -> Auto retransmit count=15
		NRF24_WriteRegister(RF_CH, _nrf24->channel);			//Init frequency channel

		uint8_t data_rate = _nrf24->data_rate;
		uint8_t power = _nrf24->power;
		uint8_t lna = _nrf24->LNA_gain;
		uint8_t setup = data_rate | power | lna;
		NRF24_WriteRegister(RF_SETUP, setup); 					//Set Power of the transmission and data rate

		NRF24_WriteRegister(STATUS, 0x0E);

		/*---------------Data Pipe Initialization--------------------*/
		NRF24_WriteRegister(RX_PW_P0, _nrf24->data_width);									//initialize data pipe 0 for ACK
		if(_nrf24->pipe == PIP1){NRF24_WriteRegister(RX_PW_P1, _nrf24->data_width);}
		if(_nrf24->pipe == PIP2){NRF24_WriteRegister(RX_PW_P2, _nrf24->data_width);}
		if(_nrf24->pipe == PIP3){NRF24_WriteRegister(RX_PW_P3, _nrf24->data_width);}
		if(_nrf24->pipe == PIP4){NRF24_WriteRegister(RX_PW_P4, _nrf24->data_width);}
		if(_nrf24->pipe == PIP5){NRF24_WriteRegister(RX_PW_P5, _nrf24->data_width);}
		if(_nrf24->pipe == ALL){
			NRF24_WriteRegister(RX_PW_P0, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P1, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P2, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P3, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P4, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P5, _nrf24->data_width);
		}
		/*-----------------------------------------------------------*/

		flush_RX();		//Reset RX-Fifo

		return 1;
	}
	if(_nrf24->mode == TX){
		HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET);

		uint8_t config = 0x0A | _nrf24->crc_scheme;
		NRF24_WriteRegister(CONFIG, config); 						//Config register

		NRF24_WriteRegister(EN_RXADDR, _nrf24->pipe); 			//enable data pipe
		NRF24_WriteRegister(EN_AA, _nrf24->pipe); 				//enable ACK for used pipe
		NRF24_WriteRegister(SETUP_AW, _nrf24->field_width);
		NRF24_WriteRegister(SETUP_RETR, 0x4F);
		NRF24_WriteRegister(RF_CH, _nrf24->channel);

		uint8_t data_rate = _nrf24->data_rate;
		uint8_t power = _nrf24->power;
		uint8_t lna = _nrf24->LNA_gain;
		uint8_t setup = data_rate | power | lna;
		NRF24_WriteRegister(RF_SETUP, setup);	//Set Power of the transmission and data rate

		NRF24_WriteRegister(STATUS, 0x0E);

		/*---------------Data Pipe Initialization--------------------*/
		NRF24_WriteRegister(RX_PW_P0, _nrf24->data_width);									//initialize data pipe 0 for ACK
		if(_nrf24->pipe == PIP1){NRF24_WriteRegister(RX_PW_P1, _nrf24->data_width);}
		if(_nrf24->pipe == PIP2){NRF24_WriteRegister(RX_PW_P2, _nrf24->data_width);}
		if(_nrf24->pipe == PIP3){NRF24_WriteRegister(RX_PW_P3, _nrf24->data_width);}
		if(_nrf24->pipe == PIP4){NRF24_WriteRegister(RX_PW_P4, _nrf24->data_width);}
		if(_nrf24->pipe == PIP5){NRF24_WriteRegister(RX_PW_P5, _nrf24->data_width);}
		if(_nrf24->pipe == ALL){
			NRF24_WriteRegister(RX_PW_P0, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P1, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P2, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P3, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P4, _nrf24->data_width);
			NRF24_WriteRegister(RX_PW_P5, _nrf24->data_width);
		}
		/*-----------------------------------------------------------*/

		flush_TX();		//Reset TX-Fifo

		return 1;
	}else{
		return 0; //error in configuration
	}
}


/*
* Function : Write data in NRF24 registers
* Return : 1=Success, 0=fail in SPI communication
* parameters : reg=register in memory; value=data
*/
uint8_t NRF24_WriteRegister(uint8_t reg, uint8_t value){
	if(reg > 0x1D){return 0;}

	uint8_t cmd = reg | 0x20;
	uint8_t status = 0;

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET); 				//Standby mode
	HAL_Delay(1);

	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
	uint8_t return1 = HAL_SPI_TransmitReceive(nrf24_spi, &cmd, &status, 1, 1000);
	uint8_t return2 = HAL_SPI_Transmit(nrf24_spi, &value, 1, 1000);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);

	if(return1==HAL_ERROR || return2==HAL_ERROR){return 0;}

	return 1;
}

/*
* Function : Read data in NRF24 registers
* Return : value read from the memory
* parameters : reg=register in memory (define in header file)
*/
uint8_t NRF24_ReadRegister(uint8_t reg){
	if(reg > 0x1D){return 0;}

	uint8_t value = 0;
	uint8_t status = 0;

	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
	HAL_SPI_TransmitReceive(nrf24_spi, &reg, &status, 1, 1000);
	HAL_SPI_Receive(nrf24_spi, &value, 1, 1000);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);

	return value;
}

/*
* Function : start receiving data (RX Mode only)
* Return : Received data; 0=error
* parameters : nrf24 object, timeout
*/
uint8_t NRF24_startReceiving(nrf24* _nrf24, uint8_t* data_buf, uint8_t timeout){
	if(timeout<=0){return 0;} //avoid bugs

	if(_nrf24->mode == RX){
		flush_RX(); 															//Reset RX-fifo
		NRF24_WriteRegister(CONFIG, 0x0B | _nrf24->crc_scheme);  									//PWR UP

		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, SET); 							//Start receiving
		HAL_Delay(50);
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET);							//Stop receiving

		uint8_t rx_dr = (NRF24_ReadRegister(STATUS) & 0x40) >> 6; 				//indicate a valid or not
		if(rx_dr==0){return 0;} 												//error code, no valid data has been received
		else{ 																	//if packet received (rx_dr=1)
			uint8_t rx_p_no = (NRF24_ReadRegister(STATUS) & 0x0E) >> 1; 		//indicate the number of data pipe
			uint8_t rx_space = 0;
			if(rx_p_no > 5){return 0;}
			if(rx_p_no == 5){rx_space = NRF24_ReadRegister(RX_PW_P5);}			//Indicate the number of bytes in RX Fifo
			if(rx_p_no == 4){rx_space = NRF24_ReadRegister(RX_PW_P4);}
			if(rx_p_no == 3){rx_space = NRF24_ReadRegister(RX_PW_P3);}
			if(rx_p_no == 2){rx_space = NRF24_ReadRegister(RX_PW_P2);}
			if(rx_p_no == 1){rx_space = NRF24_ReadRegister(RX_PW_P1);}
			if(rx_p_no == 0){rx_space = NRF24_ReadRegister(RX_PW_P0);}

			if(rx_space == 0){return 0;}

			HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);				//Read RX fifo / Payload
			NRF24_SendCommand(CMD_READ_RX);										//Send command to read
			for(int i=0; i<rx_space; i++){
				uint8_t n = 0;
				HAL_SPI_Receive(nrf24_spi, &n, 1, 1000);						//Receive byte
				data_buf[i] = n; 												//Store the byte in a table
			}
			HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);

			flush_RX();
			NRF24_WriteRegister(STATUS, 0x40);								//Reset Status Register

			return 1;															//Success
		}
	}else{
		return 0; 		//error code, not in RX mode
	}
return 0;
}

/*
* Function : Transmit data
* Return : 1=Success; 0=error (TX mode only)
* parameters : nrf24 object, data to send, length of data
*/
uint8_t NRF24_transmit(nrf24* _nrf24, uint8_t *data, uint8_t length){
	if(length==0 || data==NULL || length>32){return 0;} 						//avoid mistakes or bugs (Invalids input)
	if(_nrf24->mode == TX){
		flush_TX(); 															//reset Tx-fifo
		NRF24_WriteRegister(CONFIG, 0X0A | _nrf24->crc_scheme); 				//PWR_UP

		uint8_t status = NRF24_ReadRegister(STATUS);
		uint8_t max_rt = (status & 0x10) >> 4;									//Indicate that the maximum number of retry has been reached
		if(max_rt == 1){
			NRF24_WriteRegister(STATUS, 0x1E);										//reset mx_rt (maximum retry)
			HAL_Delay(1);
			flush_TX();																//Reset Tx Fifo
		}

		uint8_t tx_ds = (status & 0x20) >> 5;									//Indicate a success transmission (here the old transmission), So we reset it if =1
				if(tx_ds == 1){
					NRF24_WriteRegister(STATUS, 0x2E);								//reset status register
				}

		HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
		NRF24_SendCommand(CMD_WRITE_TX);										//Send command to write in the TX payload
		HAL_SPI_Transmit(nrf24_spi, data, length, 2000);  						//send payload to the NRF24
		HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);

		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, SET); 							//Start Transmission of the payload/data
		HAL_Delay(1);
		HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET);

		HAL_Delay(3);															//Wait the end of the transmission

		uint8_t Tx_full = NRF24_ReadRegister(STATUS) & 0x01;					//Indicate a full data pipe
		if(Tx_full == 1){
			flush_TX(); 															//Reset TX fifo
			return 0;
		}

		NRF24_WriteRegister(STATUS, 0x2E); 										//reset status register for next transmission
		return 1;
	}else{
		return 0;
	}
}

/*
* Function : delete data in Tx Fifo
* Return : none
* parameters : none
*/
void flush_TX(){
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
	NRF24_SendCommand(CMD_FLUSH_TX);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);
	HAL_Delay(1);
}

/*
* Function : delete data in Rx-fifo
* Return : none
* parameters : none
*/
void flush_RX(){
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
	NRF24_SendCommand(CMD_FLUSH_RX);
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);
	HAL_Delay(1);
}

/*
* Function : Send command to the NRF24 and convert data in LSB-first
* Return : status register
* parameters : command you want to send (see NRF24.h)
*/
uint8_t NRF24_SendCommand(uint8_t cmd){
	uint8_t status = 0;
	HAL_SPI_TransmitReceive(nrf24_spi, &cmd, &status, 1, 2000); //Send command
	return status;
}

/*
 * Function : Initialize address in data pipe
 * Return : 1=Success, 0=Fail
 * parameters : Address register (TX_ADDR / RX_ADDR_Px), address you want to init, Address field width (in bytes)
 */
uint8_t NRF24_Set_ADDR(uint8_t reg, uint8_t *addr, uint8_t lenght){
	if(reg != RX_ADDR_P0 || reg != RX_ADDR_P1 || reg != TX_ADDR){return 0;}
	if(lenght > 5){return 0;}

	uint8_t cmd = reg | 0x20;
	uint8_t status = 0;

	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, RESET);					//Standby mode

	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, RESET);
	HAL_SPI_TransmitReceive(nrf24_spi, &cmd, &status, 1, 1000);
	for(int i=0; i<lenght; i++){
		HAL_SPI_Transmit(nrf24_spi, &addr[i], 1, 1000);				//Send address to the register
	}
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, SET);
	return 1;
}

/*
 * Function :check the initialization and any registers in the NRF24L01 with a logical analyzer
 * Return : none
 * parameters : none
 */
void check_registers(){
	NRF24_ReadRegister(CONFIG);
	NRF24_ReadRegister(EN_AA);
	NRF24_ReadRegister(EN_RXADDR);
	NRF24_ReadRegister(SETUP_AW);
	NRF24_ReadRegister(SETUP_RETR);
	NRF24_ReadRegister(RF_CH);
	NRF24_ReadRegister(RF_SETUP);
	NRF24_ReadRegister(OBSERVE_TX);
	NRF24_ReadRegister(RX_ADDR_P0);
	NRF24_ReadRegister(RX_ADDR_P1);
	NRF24_ReadRegister(RX_ADDR_P2);
	NRF24_ReadRegister(RX_ADDR_P3);
	NRF24_ReadRegister(RX_ADDR_P4);
	NRF24_ReadRegister(RX_ADDR_P5);
	NRF24_ReadRegister(TX_ADDR);
	NRF24_ReadRegister(RX_PW_P0);
	NRF24_ReadRegister(RX_PW_P1);
	NRF24_ReadRegister(RX_PW_P2);
	NRF24_ReadRegister(RX_PW_P3);
	NRF24_ReadRegister(RX_PW_P4);
	NRF24_ReadRegister(RX_PW_P5);
	NRF24_ReadRegister(FIFO_STATUS);
	NRF24_ReadRegister(FEATURE);
}
