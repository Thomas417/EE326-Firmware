/*
 * wifi.h
 *
 * Created: 2/5/2023 10:15:35 AM
 *  Author: seanp
 */ 

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>



#define WIFI_ID_USART				ID_USART0
#define WIFI_USART					USART0
#define WIFI_USART_BAUDRATE		115200
#define USART_Handler				USART0_Handler
#define USART_IRQn					USART0_IRQn

#define ALL_INTERRUPT_MASK  0xffffffff

/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)

// Wifi COMM Pin definitions
#define WIFI_COMM_PIN_NUM			PIO_PB11
#define WIFI_COMM_PIO				PIOB
#define WIFI_COMM_ID				ID_PIOB
#define WIFI_COMM_MASK				PIO_PB11_IDX
#define WIFI_COMM_ATTR				PIO_IT_RISE_EDGE

// Wifi NET Pin definitions
#define WIFI_NET_PIN_NUM			PIO_PB12
#define WIFI_NET_PIO				PIOB
#define WIFI_NET_ID					ID_PIOB
#define WIFI_NET_MASK				PIO_PB12_IDX
//#define WIFI_NET_ATTR				PIO_IT_RISE_EDGE

// CLIENT_PIN config
#define WIFI_CLIENT_PIN_MASK			PIO_PB13_IDX
#define WIFI_CLIENT_PIN_ID				ID_PIOB
#define WIFI_CLIENT_PIN_PIO				PIOB
#define WIFI_CLIENT_PIN_NUM				PIO_PB13
//#define WIFI_CLIENT_PIN_ATTR			PIO_IT_RISE_EDGE //Double-check this

// RESET ESP32 Control pin
#define WIFI_RESET_MASK					PIO_PB0_IDX
#define WIFI_RESET_ID					ID_PIOB
#define WIFI_RESET_PIO					PIOB
#define WIFI_RESET_NUM					PIO_PB0

// WIFI_SETUP Pushbutton pin
#define WIFI_SETUP_BUTTON_MASK			PIO_PB14_IDX
#define WIFI_SETUP_BUTTON_ID            ID_PIOB
#define WIFI_SETUP_BUTTON_PIO           PIOB
#define WIFI_SETUP_BUTTON_NUM			PIO_PB14
#define WIFI_SETUP_BUTTON_ATTR          (PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE)

// Test LED Pins, for breadboard debugging
#define LED_PIN		PIO_PA19_IDX
#define LED_PIN2	PIO_PA22_IDX



// ESP32 GPIO pins (on the ESP32 NOT the MCU!)
#define ESP_COMM_GPIO					22	// Control line to MCU (ON ESP32)
#define ESP_NET_GPIO					23	// Control line to MCU (ON ESP32)
#define ESP_CLIENT_GPIO					32	// Control line to MCU (ON ESP32)


// ESP32 LED Indicator pins (on the ESP32 NOT the MCU!)
#define ESP_PROV_LED					26	// Indicator LED Pin (ON ESP32): Chip is in provisioning mode
#define ESP_NET_LED						25	// Indicator LED Pin (ON ESP32): Chip is connected to the internet
#define ESP_CLIENT_LED					27	// Indicator LED Pin (ON ESP32): Chip has at least one client connected













//DEFINE WiFi SPI parameters + pin definitions here

//Wifi buffer config		From Ilya In-Class Example: Wifi input buffer configuration
//volatile uint8_t USART_INPUT_MAX_LEN = 1000;
volatile char input_line_wifi[1000];
volatile uint32_t input_pos_wifi;

volatile bool reading_wifi_flag;
volatile bool provisioning_flag;

#define MAX_LENGTH 1000 //image length for buffer

#define SPI_CHIP_SEL 0 /* Chip select. */
#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)
#define SPI_CLK_POLARITY 0 /* Clock polarity. */
#define SPI_CLK_PHASE 0 /* Clock phase. */
#define SPI_DLYBS 0x40 /* Delay before SPCK. */
#define SPI_DLYBCT 0x10 /* Delay between consecutive transfers. */

/* SPI slave states for this example. */
#define SLAVE_STATE_IDLE           0
#define SLAVE_STATE_TEST           1
#define SLAVE_STATE_DATA           2
#define SLAVE_STATE_STATUS_ENTRY   3
#define SLAVE_STATE_STATUS         4
#define SLAVE_STATE_END            5

/* SPI example commands for this example. */
#define CMD_TEST     0x10101010 /* slave test state, begin to return RC_RDY. */
#define CMD_DATA     0x29380000 /* Slave data state, begin to return last data block. */
#define CMD_STATUS   0x68390384 /* Slave status state, begin to return RC_RDY + RC_STATUS. */
#define CMD_END      0x68390484 /* Slave idle state, begin to return RC_SYN. */
#define RC_SYN       0x55AA55AA /* General return value. */
#define RC_RDY       0x12345678 /* Ready status. */

#define CMD_DATA_MSK 0xFFFF0000 /* Slave data mask. */
#define DATA_BLOCK_MSK 0x0000FFFF /* Slave data block mask. */
#define NB_STATUS_CMD   20 /* Number of commands logged in status. */
#define NUM_SPCK_CONFIGURATIONS 4 /* Number of SPI clock configurations. */
#define COMM_BUFFER_SIZE   64 /* SPI Communicate buffer size. */
#define UART_BAUDRATE      115200 /* UART baudrate. */
#define MAX_DATA_BLOCK_NUMBER  4 /* Data block number. */
#define MAX_RETRY    4 /* Max retry times. */

//DECLARE WiFi variables here
uint8_t in_byte;
uint32_t ul_id, ul_mask;
char* comm;
uint8_t cnt;

/* Status block. */
struct status_block_t {
	uint32_t ul_total_block_number; 	/** Number of data blocks. */
	uint32_t ul_total_command_number;	/** Number of SPI commands (including data blocks). */
	uint32_t ul_cmd_list[NB_STATUS_CMD]; 	/** Command list. */
};

/* SPI clock configuration. */
static const uint32_t gs_ul_clock_configurations[] =
{ 500000, 1000000, 2000000, 5000000 };
	
static uint32_t gs_ul_spi_clock; /* SPI clock setting (Hz). */
static uint32_t gs_ul_spi_cmd; /* Current SPI return code. */
static uint32_t gs_ul_spi_state; /* Current SPI state. */

/* 64 bytes data buffer for SPI transfer and receive. */
static uint8_t gs_uc_spi_buffer[COMM_BUFFER_SIZE]; /* Transfer buffer. */
static uint8_t *gs_puc_transfer_buffer; /* Pointer to transfer buffer. */
static uint32_t gs_ul_transfer_index; /* Transfer buffer index. */
static uint32_t gs_ul_transfer_length; /* Transfer buffer length. */
static struct status_block_t gs_spi_status; /* SPI Status. */
static uint32_t gs_ul_test_block_number;

// Interrupt and Control Line Variable Initialization
volatile bool reading_wifi_flag; // Flag for test/success confirmation
volatile bool provisioning_flag; // Flag for provisioning button
volatile char input_line_wifi[1000]; // UART Data buffer
volatile uint32_t input_pos_wifi; // UART Data index variable

//DECLARE WiFi functions here
void wifi_usart_handler(void);
void wifi_provision_handler(uint32_t ul_id, uint32_t ul_mask);

void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_provision_pin(void);

void process_incoming_byte_wifi(uint8_t in_byte);
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void process_data_wifi(void);
void write_wifi_command(char* comm, uint8_t cnt);

//SPI-like functions
void wifi_spi_handler(void);
void configure_spi(void);
void spi_peripheral_initialize(void);
void prepare_spi_transfer(void);
void write_image_to_web(void);


#endif /* WIFI_H_ */