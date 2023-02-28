/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

#define CONF_BOARD_SPI /** SPI MACRO definition */
#define CONF_BOARD_SPI_NPCS0 /** SPI slave select MACRO definition */
#define SPI_ID          ID_SPI  /** Spi Hw ID . */
#define SPI_SLAVE_BASE       SPI /** SPI base address for SPI slave mode */

#define CONF_BOARD_UART_CONSOLE /** Enable Com Port. */
#define CONSOLE_UART_ID          ID_UART0 /** Usart Hw ID used by the console (UART0). */



//below: conf_board from the OV7740 image sensor example
//
#define CONF_BOARD_OV7740_IMAGE_SENSOR /** Enable OV7740 image sensor. */
#define CONF_BOARD_TWI0 /** Configure TWI0 pins (for OV7740  communications). */
#define CONF_BOARD_PCK0 /** Configure PCK0 pins (for OV7740  communications). */

/* TWI board defines. */
#define ID_BOARD_TWI                   ID_TWI0
#define BOARD_TWI                      TWI0
#define BOARD_TWI_IRQn                 TWI0_IRQn


/* Image sensor board defines. */
// Image sensor Power pin.
#define OV_POWER_PIO                   OV_SW_OVT_PIO
#define OV_POWER_MASK                  OV_SW_OVT_MASK

//// Image sensor VSYNC pin.
//#define OV7740_VSYNC_PIO	       OV_VSYNC_PIO
//#define OV7740_VSYNC_ID		       OV_VSYNC_ID
//
//#define OV7740_VSYNC_MASK              OV_VSYNC_MASK
//#define OV7740_VSYNC_TYPE              OV_VSYNC_TYPE
//
//// Image sensor data pin.
//#define OV7740_DATA_BUS_PIO            OV_DATA_BUS_PIO
//#define OV7740_DATA_BUS_ID             OV_DATA_BUS_ID




#endif // CONF_BOARD_H
