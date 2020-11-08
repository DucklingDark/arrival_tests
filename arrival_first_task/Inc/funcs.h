#define UART_BUFF_SIZE		512 	// Circular buffer size

/* 
	Structure that stores packet data
*/
typedef struct Packet{
	uint8_t	preamble;		// Preamble = 0xFF
	uint8_t cnt;			// Packets count
	uint8_t type;			// Packet type
	uint8_t length;			// Data length
	uint8_t data[255+1];	// Data part plus CRC8
}Packet_t;

void uart_routine(void); // Function with parsing RX UART bytes
uint8_t calc_crc8(uint8_t *data, uint8_t len); // Function that calculating CRC8
void send_next_byte(void); // Function that sending next processed TX byte 