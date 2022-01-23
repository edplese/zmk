/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT avago_adns5030

#include <device.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <kernel.h>
#include <drivers/sensor.h>
#include <sys/__assert.h>
#include <logging/log.h>

#include "adns5030.h"

LOG_MODULE_REGISTER(ADNS5030, CONFIG_SENSOR_LOG_LEVEL);

static int adns5030_read_reg(const struct device *dev, uint8_t reg,
			      void *data, size_t length)
{
	struct adns5030_dev_data *adns5030_data = dev->data;

	const struct spi_buf buf[2] = {
		{
			.buf = &reg,
			.len = 1
		}, {
			.buf = data,
			.len = length
		}
	};

	struct spi_buf_set tx = {
		.buffers = buf,
        .count = 1
	};

    const struct spi_buf_set rx = {
        .buffers = buf,
        .count = 2
    };

    return spi_transceive(adns5030_data->spi_master,
                    &adns5030_data->spi_config, &tx, &rx);
}

static int adns5030_sample_fetch(const struct device *dev,
				enum sensor_channel chan)
{
    struct adns5030_dev_data *data = dev->data;
    return adns5030_read_reg(dev, 0x63, &data->sample, sizeof(data->sample));
}

static int adns5030_channel_get(const struct device *dev,
			       enum sensor_channel chan,
			       struct sensor_value *val)
{
	struct adns5030_dev_data *data = dev->data;

	switch (chan) {
	case SENSOR_CHAN_POS_DX:
		val->val1 = data->sample.x;
        val->val2 = 0;
		break;
	case SENSOR_CHAN_POS_DY:
		val->val1 = data->sample.y;
        val->val2 = 0;
		break;
	default:
		return -ENOTSUP;
	}

	return 0;
}

static const struct sensor_driver_api adns5030_api_funcs = {
	.sample_fetch = adns5030_sample_fetch,
	.channel_get = adns5030_channel_get,
};

static int adns5030_init(const struct device *dev)
{
	struct adns5030_dev_data *data = dev->data;
	const struct adns5030_dev_config *cfg = dev->config;

	data->spi_master = device_get_binding(cfg->spi_master_name);

	if (!data->spi_master) {
		LOG_ERR("Failed to get SPI master\n");
		return -ENODEV;
	}

	data->spi_config.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_MODE_CPOL | SPI_MODE_CPHA;
	data->spi_config.frequency = cfg->spi_max_frequency;

	return 0;
}

static struct adns5030_dev_data adns5030_data;

static const struct adns5030_dev_config adns5030_config = {
	.spi_master_name = DT_INST_BUS_LABEL(0),
};

// TODO: Add power management
DEVICE_DT_INST_DEFINE(0, adns5030_init, device_pm_control_nop,
		    &adns5030_data, &adns5030_config, POST_KERNEL,
		    CONFIG_SENSOR_INIT_PRIORITY, &adns5030_api_funcs);
