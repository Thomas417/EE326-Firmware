/*
 * camera.h
 *
 * Created: 2/5/2023 10:16:10 AM
 *  Author: seanp
 */ 

#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include "helpers/ov2640.h"

/* Image sensor board defines. */
// Image sensor Power pin.
#define OV_POWER_PIO                   OV_SW_OVT_PIO
#define OV_POWER_MASK                  OV_SW_OVT_MASK
// Image sensor VSYNC pin.
#define OV7740_VSYNC_PIO			   OV_VSYNC_PIO
#define OV7740_VSYNC_ID				   OV_VSYNC_ID
#define OV7740_VSYNC_MASK              OV_VSYNC_MASK
#define OV7740_VSYNC_TYPE              OV_VSYNC_TYPE

// Image sensor data pin.
#define OV7740_DATA_BUS_PIO            OV_DATA_BUS_PIO
#define OV7740_DATA_BUS_ID             OV_DATA_BUS_ID


// Board-level Pin Definitions
/** OV_SW_OVT pin definition */
//#define OV_SW_OVT_GPIO                 PIO_PC10_IDX
//#define OV_SW_OVT_FLAGS                (PIO_OUTPUT_1 | PIO_DEFAULT)
//#define OV_SW_OVT_MASK                 PIO_PC10
//#define OV_SW_OVT_PIO                  PIOC
//#define OV_SW_OVT_ID                   ID_PIOC
//#define OV_SW_OVT_TYPE                 PIO_OUTPUT_1

/** OV_RST pin definition */
#define OV_RST_GPIO                    PIO_PC15_IDX
#define OV_RST_FLAGS                   (PIO_OUTPUT_1 | PIO_DEFAULT)
#define OV_RST_MASK                    PIO_PC15
#define OV_RST_PIO                     PIOC
#define OV_RST_ID                      ID_PIOC
#define OV_RST_TYPE                    PIO_OUTPUT_1

/** OV_FSIN definition */
#define OV_FSIN_GPIO                   PIO_PA21_IDX
#define OV_FSIN_FLAGS                  (PIO_OUTPUT_0 | PIO_DEFAULT)
#define OV_FSIN_MASK                   PIO_PA21
#define OV_FSIN_PIO                    PIOA
#define OV_FSIN_ID                     ID_PIOA
#define OV_FSIN_TYPE                   PIO_OUTPUT_0

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP

/** OV_VSYNC pin definition */
#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP

/** OV Data Bus pins */
#define OV_DATA_BUS_D2                 PIO_PA24_IDX
#define OV_DATA_BUS_D3                 PIO_PA25_IDX
#define OV_DATA_BUS_D4                 PIO_PA26_IDX
#define OV_DATA_BUS_D5                 PIO_PA27_IDX
#define OV_DATA_BUS_D6                 PIO_PA28_IDX
#define OV_DATA_BUS_D7                 PIO_PA29_IDX
#define OV_DATA_BUS_D8                 PIO_PA30_IDX
#define OV_DATA_BUS_D9                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT


// RST Pin (this is only driven high)
#define OV7740_RST_MASK					PIO_PA20_IDX


//DEFINE camera TWI parameters here

//DECLARE camera variables here
uint32_t ul_id, ul_mask; //things in the interrupt function definition by default
Pio *p_pio;
uint8_t *p_uc_buf; //buffer to store the image in
uint32_t ul_size; //size of the buffer

uint32_t image_size;

//DECLARE camera functions here
void vsync_handler(uint32_t ul_id, uint32_t ul_mask);
void init_vsync_interrupts(void);
void configure_twi(void);
void pio_capture_init(Pio *p_pio, uint32_t ul_id);
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *p_uc_buf, uint32_t ul_size);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);
uint8_t find_image_len(void);

#endif /* CAMERA_H_ */