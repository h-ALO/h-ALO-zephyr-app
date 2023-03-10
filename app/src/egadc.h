#pragma once
#include <zephyr/drivers/spi.h>
#include <zephyr/kernel.h>
#include "MCP356X.h"

#define ADC_MCP356X_ACQUISITION_THREAD_STACK_SIZE 1024
#define ADC_MCP356X_ACQUISITION_THREAD_PRIO 1

struct mcp356x_config
{
	const struct spi_dt_spec bus;
	struct gpio_dt_spec irq; /* GPIO  */
	struct k_sem acq_sem;		/* Signal acq thread for next sample */
	struct k_sem drdy_sem;		/* Signal data ready IRQ */
	struct gpio_callback drdy_cb;	/* For data ready IRQ */
	struct k_thread thread;		/* Acquisition thread */
	int num_irq;
	int num_drdy;

	int mv_iir[MCP356X_CHANNEL_COUNT];
	int mv_min[MCP356X_CHANNEL_COUNT];
	int mv_max[MCP356X_CHANNEL_COUNT];

	int is_scan;
	uint8_t gain_reg; /* Gain register value */
	uint16_t vref_mv; /* Voltage reference millivolt value */
	K_KERNEL_STACK_MEMBER(stack, ADC_MCP356X_ACQUISITION_THREAD_STACK_SIZE);
};


int egadc_init(struct mcp356x_config * config);
int egadc_log_REG_IRQ(const struct spi_dt_spec *bus, uint8_t reg);