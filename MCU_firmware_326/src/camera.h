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
//DEFINE camera pins here

//DEFINE camera TWI parameters here

//DECLARE camera variables here
uint32_t ul_id, ul_mask; //things in the interrupt function definition by default
Pio *p_pio;
uint8_t *p_uc_buf; //buffer to store the image in
//uint32_t ul_size; //size of the buffer

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