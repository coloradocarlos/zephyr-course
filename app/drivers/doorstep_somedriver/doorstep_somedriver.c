#define DT_DRV_COMPAT doorstep_somedriver

#include <stdio.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

/*
 * Zephyr's LOG_INF backend doesn't output to ram_console/trace0.
 * Only printf() reaches /sys/kernel/debug/remoteproc/remoteproc0/trace0.
 * Use these macros to get LOG_INF-style output through printf.
 */
#define TRACE_ERR(fmt, ...) printf("[ERR] " fmt "\n", ##__VA_ARGS__)
#define TRACE_WRN(fmt, ...) printf("[WRN] " fmt "\n", ##__VA_ARGS__)
#define TRACE_INF(fmt, ...) printf("[INF] " fmt "\n", ##__VA_ARGS__)
#define TRACE_DBG(fmt, ...) printf("[DBG] " fmt "\n", ##__VA_ARGS__)

// The OSD32MP1 Breakout Board has two LEDs: Green and Red.
// The Green LED is connected to the GPIOI pin 9 and the Red LED is connected to the GPIOI pin 8.
// The devicetree node identifier for the "led0" alias.
#define LED_NODE_0 DT_ALIAS(led0)
// Green LED configuration:
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LED_NODE_0, gpios);


static int sensor_sample_fetch_impl(const struct device *dev, enum sensor_channel chan)
{
	TRACE_INF("sensor_sample_fetch_impl for channel %d", chan);
	// Green LED toggle the GPIO
	TRACE_INF("Green LED toggle (sample fetch)");
	if (gpio_pin_toggle_dt(&led_green) < 0) return 0;
	return 0;
}

static int sensor_channel_get_impl(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	TRACE_INF("sensor_channel_get_impl for channel %d", chan);
	// Green LED toggle the GPIO
	TRACE_INF("Green LED toggle (channel get)");
	if (gpio_pin_toggle_dt(&led_green) < 0) return 0;
	val->val1 = 100;	// TODO: Replace with actual sensor value
	val->val2 = 200;	// TODO: Replace with actual sensor value
	return 0;
}

static DEVICE_API(sensor, api_doorstep_somedriver) = {
	.sample_fetch = sensor_sample_fetch_impl,
	.channel_get = sensor_channel_get_impl,
};

static int init_my_impl(const struct device *dev)
{
	TRACE_INF("init_my_impl");
	// Green LED check if the GPIO is ready
	if (!gpio_is_ready_dt(&led_green)) return 0;

	// Green LED configure the GPIO as output and active high
	if (gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_ACTIVE) < 0) return 0;


	return 0;
}

DEVICE_DT_INST_DEFINE(0, init_my_impl, NULL, NULL, NULL, POST_KERNEL,
		      CONFIG_SENSOR_INIT_PRIORITY, &api_doorstep_somedriver);
