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

//#define CONF_BOARD_UART_CONSOLE /** Enable Com Port. */
//#define CONSOLE_UART_ID          ID_UART0 /** Usart Hw ID used by the console (UART0). */

//below: conf_board from the OV7740 image sensor example
//
//#define CONF_BOARD_OV7740_IMAGE_SENSOR /** Enable OV7740 image sensor. */

//
//
/******************************* SPI definition *********************************/
/** SPI MISO pin. */
#define PIN_SPI_MISO                   {PIO_PA12A_MISO, PIOA, ID_PIOA,	\
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** SPI MOSI pin. */
#define PIN_SPI_MOSI                   {PIO_PA13A_MOSI, PIOA, ID_PIOA,	\
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** SPI SPCK pin. */
#define PIN_SPI_SPCK                   {PIO_PA14A_SPCK, PIOA, ID_PIOA,	\
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** SPI chip select pin. */
#define PIN_SPI_NPCS0                  {PIO_PA11A_NPCS0, PIOA, ID_PIOA, \
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** SPI MISO pin definition. */
#define SPI_MISO_GPIO                  (PIO_PA12_IDX)
#define SPI_MISO_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_MISO_MASK                  PIO_PA12
#define SPI_MISO_PIO                   PIOA
#define SPI_MISO_ID                    ID_PIOA
#define SPI_MISO_TYPE                  PIO_PERIPH_A
#define SPI_MISO_ATTR                  PIO_DEFAULT

/** SPI MOSI pin definition. */
#define SPI_MOSI_GPIO                  (PIO_PA13_IDX)
#define SPI_MOSI_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_MOSI_MASK                  PIO_PA13
#define SPI_MOSI_PIO                   PIOA
#define SPI_MOSI_ID                    ID_PIOA
#define SPI_MOSI_TYPE                  PIO_PERIPH_A
#define SPI_MOSI_ATTR                  PIO_DEFAULT

/** SPI SPCK pin definition. */
#define SPI_SPCK_GPIO                  (PIO_PA14_IDX)
#define SPI_SPCK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_SPCK_MASK                  PIO_PA14
#define SPI_SPCK_PIO                   PIOA
#define SPI_SPCK_ID                    ID_PIOA
#define SPI_SPCK_TYPE                  PIO_PERIPH_A
#define SPI_SPCK_ATTR                  PIO_DEFAULT

/** SPI chip select 0 pin definition. */
#define SPI_NPCS0_GPIO                 (PIO_PA11_IDX)
#define SPI_NPCS0_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define SPI_NPCS0_MASK                 PIO_PA11
#define SPI_NPCS0_PIO                  PIOA
#define SPI_NPCS0_ID                   ID_PIOA
#define SPI_NPCS0_TYPE                 PIO_PERIPH_A
#define SPI_NPCS0_ATTR                 PIO_DEFAULT
//
///******************************* TWI definition*********************************/
///** TWI0 data pin */
//#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, \
                                         //PIO_PERIPH_A, PIO_DEFAULT}
//
///** TWI0 clock pin */
//#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	\
                                         //PIO_PERIPH_A, PIO_DEFAULT}
//
///** TWI0 Data pins definition */
//#define TWI0_DATA_GPIO                 PIO_PA3_IDX
//#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
//#define TWI0_DATA_MASK                 PIO_PA3
//#define TWI0_DATA_PIO                  PIOA
//#define TWI0_DATA_ID                   ID_PIOA
//#define TWI0_DATA_TYPE                 PIO_PERIPH_A
//#define TWI0_DATA_ATTR                 PIO_DEFAULT
//
///** TWI0 clock pins definition */
//#define TWI0_CLK_GPIO                  PIO_PA4_IDX
//#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
//#define TWI0_CLK_MASK                  PIO_PA4
//#define TWI0_CLK_PIO                   PIOA
//#define TWI0_CLK_ID                    ID_PIOA
//#define TWI0_CLK_TYPE                  PIO_PERIPH_A
//#define TWI0_CLK_ATTR                  PIO_DEFAULT
//
///** TWI0 pins */
//#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0
//
//#define ID_BOARD_TWI		               ID_TWI0
//#define BOARD_TWI			                 TWI0
//#define BOARD_TWI_IRQn		             TWI0_IRQn
//
//#define CONF_BOARD_TWI0 /** Configure TWI0 pins (for OV7740  communications). */
//#define CONF_BOARD_PCK0 /** Configure PCK0 pins (for OV7740  communications). */
//
///* TWI board defines. */
//#define ID_BOARD_TWI                   ID_TWI0
//#define BOARD_TWI                      TWI0
//#define BOARD_TWI_IRQn                 TWI0_IRQn



#endif // CONF_BOARD_H
