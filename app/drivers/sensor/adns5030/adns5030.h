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
};

struct adns5030_dev_config {
	const char *spi_master_name;
	uint32_t spi_max_frequency;
};