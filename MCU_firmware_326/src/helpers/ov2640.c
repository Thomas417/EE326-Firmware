/**
 * \file
 *
 * \brief API driver for OV2640 CMOS image sensor.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "helpers/ov2640.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup OV2640_CMOS_image_sensor_group
 *
 * Low-level driver for the OV2640 CMOS image sensor. This driver provides
 * access to the main features of the  OV2640 CMOS image sensor.
 *
 * @{
 */

/**
 * \brief Read PID and VER.
 *
 * \param p_twi TWI interface.
 * \return PID and VER.
 */
static uint32_t ov_id(Twi* const p_twi)
{
	twi_packet_t packet_pid;
	twi_packet_t packet_ver;
	uint32_t ul_id = 0;
	uint32_t ul_ver = 0;

	/* OV_PID */
	packet_pid.chip = OV_I2C_SENSOR_ADDRESS;
	packet_pid.addr[0] = OV2640_PIDH;
	packet_pid.addr_length = 1;
	packet_pid.buffer = &ul_id;
	packet_pid.length = 1;

	ov_read_reg(p_twi, &packet_pid);

	/* OV_VER */
	packet_ver.chip = OV_I2C_SENSOR_ADDRESS;
	packet_ver.addr[0] = OV2640_PIDL;
	packet_ver.addr_length = 1;
	packet_ver.buffer = &ul_ver;
	packet_ver.length = 1;

	ov_read_reg(p_twi, &packet_ver);
	return ((uint32_t)(ul_id << 8) | ul_ver);
}

/**
 * \brief Read Manufacturer.
 *
 * \param p_twi TWI interface.
 * \return 0 if the sensor is present, 1 otherwise.
 */
static uint32_t ov_manufacturer(Twi* const p_twi)
{
	twi_packet_t twi_packet;
	uint32_t ul_midh = 0;
	uint32_t ul_midl = 0;

	/* OV_MIDH */
	twi_packet.addr[0] = OV2640_MIDH;
	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.buffer = &ul_midh;
	twi_packet.length = 1;

	ov_read_reg(p_twi, &twi_packet);

	/* OV_MIDL */
	twi_packet.addr[0] = OV2640_MIDL;
	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.buffer = &ul_midl;
	twi_packet.length = 1;

	ov_read_reg(p_twi, &twi_packet);

	if ((ul_midh == OV2640_MIDH_DEFAULT) && (ul_midl == OV2640_MIDL_DEFAULT)) {
		return 0;
	}

	return 1;
}

/**
 * \brief OV2640 test write.
 *
 * \param p_twi TWI interface.
 * \return 0 on success, 1 otherwise.
 */
static uint32_t ov_test_write(Twi* const p_twi)
{
	twi_packet_t twi_packet;
	uint32_t ul_value = 0;
	uint32_t ul_oldvalue = 0;
	uint32_t ul_entervalue = 0;

	/* OV_BLUE_GAIN */
	twi_packet.addr[0] = 0x01;
	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.length = 1;

	twi_packet.buffer = &ul_oldvalue;
	ov_read_reg(p_twi, &twi_packet);

	ul_entervalue = 0xAD;
	twi_packet.buffer = &ul_entervalue;
	ov_write_reg(p_twi, &twi_packet);

	twi_packet.buffer = &ul_value;
	ov_read_reg(p_twi, &twi_packet);

	if (ul_value != ul_entervalue) {
		return 1;
	}

	/* return old value */
	twi_packet.buffer = &ul_oldvalue;
	ov_write_reg(p_twi, &twi_packet);

	twi_packet.buffer = &ul_value;
	ov_read_reg(p_twi, &twi_packet);

	if (ul_value != ul_oldvalue) {
		return 1;
	}

	return 0;
}

/**
 * \brief Register array for manual operation
 * (given by OmniVision toolkit).
 */
static ov_reg regs_manual[] = {
	{0x13, 0x0},
	{0x0f, 0x0},
	{0x10, 0x0},
	{0x80, 0x0},
	{0x01, 0x0},
	{0x02, 0x0},
	{0x03, 0x0},
	{0xff, 0xff}
};

/**
 * \brief Retrieve register manually.
 *
 * \param p_twi TWI interface.
 */
static void ov_retrieve_manual(Twi* const p_twi)
{
	uint8_t i = 0;
	twi_packet_t twi_packet;

	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.length = 1;

	for (; i < sizeof(regs_manual) / sizeof(ov_reg); i++) {
		twi_packet.addr[0] = regs_manual[i].reg;
		twi_packet.buffer = &regs_manual[i].val;

		ov_read_reg(p_twi, &twi_packet);
		switch (regs_manual[i].reg) {
		case 0x13:
			/* no ae */
			regs_manual[i].val = 0x0;
			break;

		case 0x80:
			/* no ae */
			regs_manual[i].val = 0x0F;
			break;

		case 0xff:
			/* no ae */
			regs_manual[i].val = 0xff;
			break;
		}
	}
}

/**
 * \brief Turn OV2640 sensor on/off using power pin.
 *
 * \param on_off 1 to turn on OV2640 sensor and 0 to turn off.
 * \param p_pio Pointer to the PIO instance which control OV2640 sensor power.
 * \param ul_mask Bitmask of OV2640 sensor power pin.
 */
void ov_power(uint32_t ul_on_off, Pio* const p_pio, const uint32_t ul_mask)
{
	if (ul_on_off) {
		pio_clear(p_pio, ul_mask);
	} else {
		pio_set(p_pio, ul_mask);
	}
}

/**
 * \brief Reset OV2640 sensor using power pin.
 *
 * \param p_pio Pointer to the PIO instance which controls the OV2640 sensor
 * power pin.
 * \param ul_mask Bitmask of the OV2640 sensor power pin.
 */
void ov_reset(Pio* const p_pio, const uint32_t ul_mask)
{
	pio_clear(p_pio, ul_mask);
	delay_ms( 2 );

	pio_clear(p_pio, ul_mask);
	delay_ms( 2 );
}

/**
 * \brief Read a value from a register in an OV2640 sensor device.
 *
 * \param p_twi TWI interface.
 * \param p_packet TWI packet.
 * \return 0 on success, TWID_ERROR_BUSY otherwise.
 */
uint32_t ov_read_reg(Twi* const p_twi, twi_packet_t* const p_packet)
{
	uint32_t ul_status;

	ul_status = twi_master_read(p_twi, p_packet);

	return ul_status;
}

/**
 * \brief Write a specified value to a register of the OV2640 sensor.
 *
 * \param p_twi TWI interface.
 * \param p_packet TWI packet.
 * \return 0 on success, TWID_ERROR_BUSY otherwise.
 */
uint32_t ov_write_reg(Twi* const p_twi, twi_packet_t* const p_packet)
{
	uint32_t ul_status;

	ul_status = twi_master_write(p_twi, p_packet);

	return ul_status;
}

/**
 * \brief Initialize a list of OV2640 registers.
 * The list of registers is terminated by the pair of values
 * {OV_REG_TERM, OV_VAL_TERM}.
 *
 * \param p_twi TWI interface.
 * \param p_reg_list Register list to be written.
 * \return 0 on success, TWID_ERROR_BUSY otherwise.
 */
uint32_t ov_write_regs(Twi* const p_twi, const ov_reg *p_reg_list)
{
	uint32_t ul_err;
	uint32_t ul_size = 0;
	twi_packet_t twi_packet_regs;
	ov_reg *p_next = (ov_reg *)p_reg_list;

	while (!((p_next->reg == OV_REG_TERM) &&
			(p_next->val == OV_VAL_TERM))) {
		if (p_next->reg == 0xFE) {
			delay_ms(5);
		} else {
			twi_packet_regs.addr[0] = p_next->reg;
			twi_packet_regs.addr_length = 1;
			twi_packet_regs.chip = OV_I2C_SENSOR_ADDRESS;
			twi_packet_regs.length = 1;
			twi_packet_regs.buffer = &(p_next->val);

			ul_err = ov_write_reg(p_twi, &twi_packet_regs);
			ul_size++;

			if (ul_err == TWI_BUSY) {
				return ul_err;
			}
		}

		p_next++;
	}
	return 0;
}

/**
 * \brief Dump all registers.
 *
 * \param p_twi TWI interface.
 * \param p_regs Register list to be dumped.
 */
void ov_dump_registers(Twi* const p_twi, ov_reg *p_regs)
{
	uint32_t i;
	uint32_t ul_value;
	twi_packet_t twi_packet;
	uint32_t ul_reg_num;

	ul_reg_num = OV2640_YUV422CTRL;

	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.length = 1;
	twi_packet.buffer = &ul_value;

	for (i = 0; i <= ul_reg_num; i++) {
		ul_value = 0;
		twi_packet.addr[0] = i;
		ov_read_reg(p_twi, &twi_packet);

		if (p_regs != NULL) {
			p_regs[i].reg = i;
			p_regs[i].val = ul_value;
		}
	}
}

/**
 * \brief Initialize the OV2640 sensor.
 *
 * \param p_twi TWI interface.
 * \return 0 on success, 1 otherwise.
 */
uint32_t ov_init(Twi* const p_twi)
{
	const uint8_t cont_reg_val = 1;
	twi_packet_t init_packet = {
		.addr         = 0xFF,      // TWI slave memory address data
		.addr_length  = 1, //sizeof (uint16_t),    // TWI slave memory address data size
		.chip         = OV_I2C_SENSOR_ADDRESS,      // TWI slave bus address
		.buffer       = &cont_reg_val,        // transfer data destination buffer
		.length       = 1                    // transfer data size (bytes)
	};
	ov_write_reg(p_twi, &init_packet);
	
	
	uint32_t ul_id = 0;

	ul_id = ov_id( p_twi );

	if (((ul_id >> 8)&0xff)  == OV2640_PIDH_DEFAULT) {
		
		if (ov_manufacturer( p_twi ) == 0) {
			
			if (ov_test_write( p_twi ) == 0) {
				
				return 0;
			}
		}
	}

	return 1;
}

/**
 * \brief Configure the OV2640 sensor for a specified image size (pixel format).
 *
 * \param p_twi TWI interface.
 * \param format Specific format to configure.
 * \return 0 on success, 1 otherwise.
 */
uint32_t ov_configure(Twi* const p_twi, const e_OV2640_format format)
{
	const ov_reg *p_regs_conf = NULL;

	/* Common register initialization */
	switch (format) {
		
	case JPEG_INIT:
		p_regs_conf = OV2640_JPEG_INIT;
		break;
		
	case YUV422:
		p_regs_conf = OV2640_YUV422;
		break;
		
	case JPEG:
		p_regs_conf = OV2640_JPEG;
		break;
		
	case JPEG_320x240:
		p_regs_conf = OV2640_JPEG_320x240;
		break;
		
	case JPEG_640x480:
		p_regs_conf = OV2640_JPEG_640x480;
		break;
		
	case JPEG_800x600:
		p_regs_conf = OV2640_JPEG_800x600;
		break;
		
	case JPEG_1024x768:
		p_regs_conf = OV2640_JPEG_1024x768;
		break;
		
	case JPEG_1280x1024:
		p_regs_conf = OV2640_JPEG_1280x1024;
		break;
		
	case JPEG_1600x1200:
		p_regs_conf = OV2640_JPEG_1600x1200;
		break;
		
	case QVGA_YUV422_10FPS:
		p_regs_conf = OV2640_QVGA_YUV422_10FPS;
		break;

	case QVGA_YUV422_15FPS:
		p_regs_conf = OV2640_QVGA_YUV422_15FPS;
		break;

	case QVGA_YUV422_20FPS:
		p_regs_conf = OV2640_QVGA_YUV422_20FPS;
		break;

	case QVGA_YUV422_30FPS:
		p_regs_conf = OV2640_QVGA_YUV422_30FPS;
		break;

	case QVGA_RGB888:
		p_regs_conf = OV2640_QVGA_RGB888;
		break;

	case QQVGA_YUV422:
		p_regs_conf = OV2640_QQVGA_YUV422;
		break;

	case QQVGA_RGB888:
		p_regs_conf = OV2640_QQVGA_RGB888;
		break;

	case TEST_PATTERN:
		p_regs_conf = OV2640_TEST_PATTERN;
		break;

	case VGA_YUV422_20FPS:
		p_regs_conf = OV2640_VGA_YUV422_20FPS;
		break;

	default:
		break;
	}

	if (p_regs_conf != NULL) {
		ov_write_regs( p_twi, p_regs_conf );
		return 0;
	}

	return 1;
}

/**
 * \brief Complete the OV2640 sensor configuration.
 *
 * \param p_twi TWI interface.
 */
uint32_t ov_configure_finish(Twi* const p_twi)
{
	twi_packet_t twi_packet;
	uint32_t ul_value = 0xff;

	/* OV_MIDH */
	twi_packet.addr[0] = 0xff;
	twi_packet.addr_length = 1;
	twi_packet.chip = OV_I2C_SENSOR_ADDRESS;
	twi_packet.buffer = &ul_value;
	twi_packet.length = 1;

	ov_write_reg(p_twi, &twi_packet);

	return 0;
}

/**
 * \brief Configure the OV2640 sensor manually.
 *
 * \param p_twi TWI interface.
 */
uint32_t ov_configure_manual(Twi* const p_twi)
{
	ov_write_regs(p_twi, regs_manual);

	return 0;
}

/**
 * \brief Store back up register manually.
 *
 * \param p_twi TWI interface.
 * \param p_backup_addr Backup register address.
 *
 * \return ul_size on success, 0 otherwise.
 */
uint32_t ov_store_manual(Twi* const p_twi, volatile uint32_t *p_backup_addr,
		uint32_t ul_size)
{
	uint32_t ul_offset = 0;

	if (sizeof(regs_manual) / sizeof(ov_reg) <= ul_size *
			sizeof(uint32_t)) {
		/* retrieve values from image sensor */
		ov_retrieve_manual(p_twi);
		while (ul_offset < ul_size) {
			*(p_backup_addr + ul_offset) = 0x0;
			for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
				*(p_backup_addr+ul_offset) |= ((regs_manual[i+ul_offset*
						sizeof(uint32_t)].val&0xff) << (i*8));
			}
			ul_offset++;
		}
		return ul_size;
	} else {
		return 0;
	}
}

/**
 * \brief Restore backup register manually.
 *
 * \param p_backup_addr Backup register address.
 *
 * \return ul_size on success, 0 otherwise.
 */
uint32_t ov_restore_manual(volatile uint32_t *p_backup_addr, uint32_t ul_size)
{
	uint32_t ul_offset = 0;

	if (sizeof(regs_manual) / sizeof(ov_reg) <= ul_size * sizeof(uint32_t)) {
		while (ul_offset < ul_size) {
			for (uint8_t i = 0; i < sizeof(uint32_t); i++) {
				regs_manual[i + ul_offset * 	sizeof(uint32_t)].val	=
						(((*(p_backup_addr + ul_offset)) >> (i * 8)) & 0xFF);
			}
			ul_offset++;
		}
		return ul_size;
	} else {
		return 0;
	}
}

/* @} */

#ifdef __cplusplus
}
#endif
