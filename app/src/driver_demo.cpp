#include <stdio.h>

#include <zephyr/autoconf.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#if IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)
#include <zephyr/drivers/sensor.h>
#endif

/*
 * Zephyr's LOG_INF backend doesn't output to ram_console/trace0.
 * Only printf() reaches /sys/kernel/debug/remoteproc/remoteproc0/trace0.
 * Use these macros to get LOG_INF-style output through printf.
 */
#define TRACE_ERR(fmt, ...) printf("[ERR] " fmt "\n", ##__VA_ARGS__)
#define TRACE_WRN(fmt, ...) printf("[WRN] " fmt "\n", ##__VA_ARGS__)
#define TRACE_INF(fmt, ...) printf("[INF] " fmt "\n", ##__VA_ARGS__)
#define TRACE_DBG(fmt, ...) printf("[DBG] " fmt "\n", ##__VA_ARGS__)

/* The devicetree node identifier for the "led0" and "led1" aliases. */
#define LED_NODE_0 DT_ALIAS(led0)
#define LED_NODE_1 DT_ALIAS(led1)

// The OSD32MP1 Breakout Board has two LEDs: Green and Red.
// The Green LED is connected to the GPIOI pin 9 and the Red LED is connected to the GPIOI pin 8.
// Green LED configuration:
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LED_NODE_0, gpios);
// Red LED configuration:
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LED_NODE_1, gpios);

namespace {

void test(void)
{
#if IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)
	TRACE_INF("doorstep_somedriver probe");
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));
	if (dev == NULL) {
		TRACE_ERR("Device not found");
		return;
	}
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, NULL);
	if (ret < 0) {
		TRACE_ERR("Failed to get sensor value");
		return;
	}
	TRACE_INF("Sensor channel_get returned %d", ret);
#else
	TRACE_INF("doorstep_somedriver disabled in configuration");
#endif
}

} /* namespace */

int main(void)
{
	test();
	bool led_state = true;
	uint32_t count = 0;

	// Green LED check if the GPIO is ready
	if (!gpio_is_ready_dt(&led_green)) return 0;
	// Red LED check if the GPIO is ready
	if (!gpio_is_ready_dt(&led_red)) return 0;

	// Green LED configure the GPIO as output and active high
	if (gpio_pin_configure_dt(&led_green, GPIO_OUTPUT_ACTIVE) < 0) return 0;
	// Red LED configure the GPIO as output and active high
	if (gpio_pin_configure_dt(&led_red, GPIO_OUTPUT_ACTIVE) < 0) return 0;

	printk("LED application started - trace0 enabled\n");

	while (1) {
		// Green LED toggle the GPIO
		if (gpio_pin_toggle_dt(&led_green) < 0) return 0;

		led_state = !led_state;
		TRACE_INF("(%d) LED state: %s", ++count, led_state ? "ON" : "OFF");
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);

		// Red LED toggle the GPIO
		if (gpio_pin_toggle_dt(&led_red) < 0) return 0;
		k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
	}
	return 0;
}
