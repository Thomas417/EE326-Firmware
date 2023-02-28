/*
 * camera.c
 *
 * Created: 2/5/2023 10:15:57 AM
 *  Author: seanp
 */ 

#include "camera.h"

//INITIALIZE camera variables here
/* LCD board defines. */
#define ILI9325_LCD_CS                 (2UL) // Chip select number
#define IMAGE_WIDTH                    (320UL)
#define IMAGE_HEIGHT                   (240UL)

#define TWI_CLK     (400000UL) /* TWI clock frequency in Hz (400KHz) */
uint8_t *g_p_uc_cap_dest_buf; /* Pointer to the image data destination buffer */
uint16_t g_us_cap_rows = IMAGE_HEIGHT; /* Rows size of capturing picture */
ul_size = 100000000; 

/* Define display function and line size of captured picture according to the */
/* current mode (color or black and white) */
#define _display() draw_frame_yuv_color_int()

/* (IMAGE_WIDTH *2 ) because ov7740 use YUV422 format in color mode */
/* (draw_frame_yuv_color_int for more details) */
uint16_t g_us_cap_line = (IMAGE_WIDTH * 2);

/* Push button information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_push_button_trigger = false;

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

//DEFINE camera functions here
void vsync_handler(uint32_t ul_id, uint32_t ul_mask){
	//Handler for rising-edge of VSYNC signal. 
	//Should set a flag indicating a rising edge of VSYNC.
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

void init_vsync_interrupts(void){
	//Configuration of VSYNC interrupt.
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	//pio_handler_set(OV7740_VSYNC_PIO, OV7740_VSYNC_ID, OV7740_VSYNC_MASK,
			//OV7740_VSYNC_TYPE, vsync_handler);


	//sample style of what the different terms should look like
	//see conf_board.h in sample project for the 7740 stuff
	//#define OV2640_VSYNC_GPIO //PIO_PA15_IDX
	//#define OV2640_VSYNC_PIO //PIOA
	//#define OV2640_VSYNC_ID //ID_PIOA
	//#define OV2640_VSYNC_MASK //PIO_PA15
	//#define OV2640_VSYNC_TYPE //PIO_PULLUP
	

	/* Enable PIO controller IRQs */
	//NVIC_EnableIRQ((IRQn_Type)OV7740_VSYNC_ID);
}

void configure_twi(void){
	//Configuration of TWI (two wire interface)
	
}

void init_camera(void){
	//Configuration of camera pins, camera clock (XCLK), and
	//calling the configure_twi() function.
	
}

void configure_camera(void){
	//Configuration of OV2640 registers for desired operation.
	//ov_configure(BOARD_TWI, JPEG_INIT);
	//ov_configure(BOARD_TWI, YUV422);
	//ov_configure(BOARD_TWI, JPEG);
	//ov_configure(BOARD_TWI, JPEG_320x240);
	//
	//there may be more to it than this
}

void pio_capture_init(Pio *p_pio, uint32_t ul_id){
	//Configuration and initialization of parallel
	//capture.
	
	/* Enable peripheral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable PIO capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable RXBUFF interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
	
	//removed a black-and-white provision
}

uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size){
	//Uses parallel	capture and PDC to store image in buffer.
	
		/* Check if the first PDC bank is free */
		if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
			p_pio->PIO_RPR = (uint32_t)uc_buf;
			p_pio->PIO_RCR = ul_size;
			p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
			return 1;
			
		} else if (p_pio->PIO_RNCR == 0) {
			p_pio->PIO_RNPR = (uint32_t)uc_buf;
			p_pio->PIO_RNCR = ul_size;
			return 1;
			
		} else {
			return 0;
		}
}

uint8_t start_capture(void){
	//Captures an image after a rising edge of VSYNC, and gets image
	//length. Returns 1 on success (i.e. a nonzero image length), 0 on error.
	while (!g_ul_vsync_flag) {
		//do nothing
	}
	g_ul_vsync_flag = false;

	pio_capture_to_buffer(p_pio, p_uc_buf, ul_size);
	uint8_t len_success = find_image_len();
	return (ul_size > 0);
	
}

uint8_t find_image_len(void){
	//Finds image length based on JPEG protocol. Returns 1 on success
	//(i.e. able to find “end of image” and “start of image” markers),
	// 0 on error.
	
	//iterate through the buffer and look for start and end of image markers
	//start is 0xFFD8; end is 0xFFD9
	uint8_t image_started = 0; //logical T/F
	uint8_t image_ended = 0;
	image_size = 0;
	uint8_t byte;
	
	for (uint32_t i = 0; i < ul_size; ++i){
				
		//look for start of image
		byte = p_uc_buf[i];
		if (byte == 0xFFD8) {
			image_started = 1;
		} else if (byte == 0xFFD9) {
			image_ended = 1;
			break;
		} else if (image_started) {
			image_size++; 			//add +1 to the length counter
		}
	}
	
	//return 0 if start or end markers weren't encountered, 1 otherwise
	return (!image_started || !image_ended);
	
}

