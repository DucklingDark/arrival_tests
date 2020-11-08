#include <funcs.h>

extern UART_HandleTypeDef huart4; // huart4 is a basic timer structure that stores UART configuration and other parameters. Code generated by STM32CubeIDE

Packet_t packet;			// Variable for Packet structure
uint8_t state = 0;			// Variable for state machine stage
uint8_t crc8 = 0;			// Variable for calculated CRC8
uint16_t processed = 0;		// Variable for processed byte position
uint16_t received = 0;		// Variable for received byte position
uint8_t rx_buffer[UART_BUFF_SIZE] = {0xff};	// Circular array for RX buffer bytes
uint8_t new_processed = 0;	// Variable for new successfull received and prepared packet
uint8_t n_byte = 0;			// Variable for new received byte
uint16_t data_cnt = 0;		// Variable for processing data buffer position
uint16_t tx_pos = 0;		// Variable for TX buffer byte position
uint8_t *tx_buf = NULL;		// Variable for TX buffer
uint16_t packet_size = 0;	// Variable for TX buffer size calculation

/*
	Function that check if there are bytes that not processed
*/
uint8_t is_not_processed(void){
	return (UART_BUFF_SIZE + received - processed) % UART_BUFF_SIZE; 
}

/*
	Function with parsing RX UART bytes
*/
void uart_routine(void){
	while (is_not_processed() != 0){ 	// Check if there is any data need to process
		n_byte = rx_buffer[processed];	// Take a new byte

		switch (state){					// State machine
			case 0: 					// State 0. Check preamble
				if (n_byte == 0xFF){	// If new byte == 0xFF
					packet.preamble = n_byte;	// Write preamble into structure
					data_cnt = 0;		// Reset data count
					state = 1;			// Set state = 1
				}
				break;
			case 1: 					// State 1. Packets count
				packet.cnt = n_byte;	// Write packets count into structure
				state = 2;				// Set state = 2
				break;
			case 2: 					// State2. Packet type
				packet.type = n_byte;	// Write packet type into structure
				state = 3;				// Set state = 3
				break;
			case 3:						// State 3. Data length
				packet.length = n_byte; // Write data length into structure
				state = 4;				// Set state = 4
				break;
			case 4: 					// State 4. Data part
				packet.data[data_cnt] = n_byte; // Write new byte into byte array in structure
				data_cnt++;				// Increse data count variable

				if (data_cnt == packet.length){	// Check if data count equal data length of data part from packet
					state = 5;			// Set state = 5
				}
				break;
			case 5: 					// State 5. Check CRC8
				if (new_processed == 0){ // Check if last packet is processed and transmitted
					packet_size = packet.length + 5;		// Calculate packet size that equal data length + 5 bytes for preamble, count, type, length and CRC8
					tx_buf = (uint8_t*)malloc(packet_size);	// Allocate memory for TX buffer
					tx_buf[0] = packet.preamble;			// Set preamble
					tx_buf[1] = packet.cnt;					// Set count
					tx_buf[2] = packet.type;				// Set type
					tx_buf[3] = packet.length;				// Set length

					for (int i = 4; i < packet_size - 1; i++){
						tx_buf[i] = packet.data[i - 4];		// Add all data part
					}

					crc8 = calc_crc8(*tx_buf, packet_size - 1);	// Calculate CRC8

					if (n_byte == crc8){	// Check if the calculated CRC8 is equal CRC8 from packet
						tx_buf[2] |= 0x80;	// Set High-order bit of type in 1
						tx_buf[packet_size - 1] = crc8;	// Add CRC8
						new_processed = 1;	// Set flag of new packet that need to transmit
					}
					state = 0;	// Set state = 0
				}
				break;
		}

		processed++;	// Increase processed byte variable

		if (processed == UART_BUFF_SIZE){	// Check if processed byte variable equal defined UART buffer size
			processed = 0;	// Set processed byte variable = 0
		}
	}
}

/*
	Function that calculating CRC8
*/
uint8_t calc_crc8(uint8_t *data, uint16_t len){
	uint8_t crc = 0xFF;
	uint8_t i = 0;

	while (len--)
	{
		crc ^= *data++;

		for (i = 0; i < 8; i++)
			crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
	}

	return crc;
}

/*
	Function that sending next processed TX byte 
*/
void send_next_byte(void){
	if (new_processed){	// Check if there is a new packet that processed and need to transmit
		if (huart4.Instance->SR & UART_FLAG_TXE){	// Check if UART TX Enable (TXE flag)
			huart4.Instance->DR = tx_buf[tx_pos];	// Write new bbyte into Data Register (DR) of UART
			tx_pos++;								// Increase TX byte position

			if (tx_pos == packet_size){				// Check if TX byte position is equal size of the packet
				tx_pos = 0;							// Set TX byte position = 0
				new_processed = 0;					// Set processed packet flag = 0
			}
		}
	}
}
