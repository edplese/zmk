/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <device.h>
#include <drivers/spi.h>
#include <sys/util.h>

struct adns5030_sample {
	int8_t x;
	int8_t y;
};

struct adns5030_dev_data {
	const struct device *spi_master;
	struct spi_config spi_config;
	struct adns5030_sample sample;

#if DT_INST_SPI_DEV_HAS_CS_GPIOS(0)
	struct spi_cs_control adns5030_cs_ctrl;
#endif
};

struct adns5030_dev_config {
	const char *spi_master_name;
	uint16_t spi_slave;
	uint32_t spi_max_frequency;
#if DT_INST_SPI_DEV_HAS_CS_GPIOS(0)
	const char *gpio_cs_port;
	gpio_pin_t cs_gpio;
	gpio_dt_flags_t cs_flags;
#endif
};