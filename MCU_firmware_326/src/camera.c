/*
 * camera.c
 *
 * Created: 2/5/2023 10:15:57 AM
 *  Author: seanp
 */ 

#include "camera.h"

//INITIALIZE camera variables here
/* LCD board defines. */
//#define ILI9325_LCD_CS                 (2UL) // Chip select number
#define IMAGE_WIDTH                    (320UL)
#define IMAGE_HEIGHT                   (240UL)

#define TWI_CLK     (400000UL) /* TWI clock frequency in Hz (400KHz) */
uint8_t *g_p_uc_cap_dest_buf; /* Pointer to the image data destination buffer */
uint16_t g_us_cap_rows = IMAGE_HEIGHT; /* Rows size of capturing picture */
//ul_size = 100000000; 

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
	pio_handler_set(OV7740_VSYNC_PIO, OV7740_VSYNC_ID, OV7740_VSYNC_MASK,
			OV7740_VSYNC_TYPE, vsync_handler);	

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV7740_VSYNC_ID);
}

void configure_twi(void){
	//Configuration of TWI (two wire interface)
	twi_options_t opt;

	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);

	/* ov7740 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}
}

void init_camera(void){
	//Configuration of camera pins, camera clock (XCLK), and
	//calling the configure_twi() function.
	configure_twi();
	
	//configure camera pins
	//configure camera clock
	
	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);

	///* Turn on ov7740 image sensor using power pin */
	//ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	//PMC->PMC_PCK[0] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLA_CLK);
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLA_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}
	
	pmc_enable_pllack(7, 0x1, 1); /* PLLA work at 96 Mhz */
	
	configure_twi();
	
	// RST Pin Management
	ioport_set_pin_dir(OV7740_RST_MASK,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OV7740_RST_MASK,true);
	
}

void configure_camera(void){
	//Configuration of OV2640 registers for desired operation.
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_320x240);
	
	//there may be more to it than this

}

void pio_capture_init(Pio *p_pio, uint32_t ul_id){
	//Configuration and initialization of parallel
	//capture.
	
	///* Enable peripheral clock */
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
	
	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV7740_VSYNC_PIO, OV7740_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV7740_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV7740_DATA_BUS_PIO, g_p_uc_cap_dest_buf,
			(g_us_cap_line * g_us_cap_rows) >> 2);

	/* Wait end of capture*/
	while (!((OV7740_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture*/
	pio_capture_disable(OV7740_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	/* Check Size  */
	uint8_t len_success = find_image_len();
	return (image_size > 0);
	
}

uint8_t find_image_len(void){
	//Finds image length based on JPEG protocol. Returns 1 on success
	//(i.e. able to find �end of image� and �start of image� markers),
	// 0 on error.
	
	//iterate through the buffer and look for start and end of image markers
	//start is 0xFFD8; end is 0xFFD9
	uint8_t image_started = 0; //logical T/F
	uint8_t image_ended = 0;
	image_size = 0;
	uint8_t byte1, byte2;
	
	//for (uint32_t i = 0; i < ul_size; ++i){
				//
		////need to check 2 consecutive bytes
				//
				//
		////look for start of image
		//byte = p_uc_buf[i];
		//if (byte == 0xFFD8) {
			//image_started = 1;
		//} else if (byte == 0xFFD9) {
			//image_ended = 1;
			//break;
		//} else if (image_started) {
			//image_size++; 			//add +1 to the length counter
		//}
	//}
	
	for (uint32_t i = 0; i < (int)ul_size/2; ++i){
			
			//need to check 2 consecutive bytes
			byte1 = p_uc_buf[2*i];
			byte2 = p_uc_buf[2*i+1];
			
			//look for start of image
			if (byte1 == 0xFF && byte2 == 0xD8){
				image_started = 1;
			} else if (byte1 == 0xFF && byte2 == 0xD9) {
				image_ended = 1;
				break;
			} else if (image_started) {
				image_size++; 			//add +1 to the length counter
			}
		}
	
	//return 0 if start or end markers weren't encountered, 1 otherwise
	return (!image_started || !image_ended);
	
}

