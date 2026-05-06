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

void call_driver_api(void)
{
#if IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)
	TRACE_INF("doorstep_somedriver probe");
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));
	if (dev == NULL) {
		TRACE_ERR("Device not found");
		return;
	}

	// Fetch sensor value
	int ret = sensor_sample_fetch(dev);
	if (ret < 0) {
		TRACE_ERR("Failed to fetch sensor value");
		return;
	}
	TRACE_INF("Sensor driver: sensor_sample_fetch returned %d", ret);

	// Sleep for half a second to allow the LED to visually toggle
	k_msleep(500);

	// Get sensor value for all channels
	struct sensor_value val;
	ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);
	if (ret < 0) {
		TRACE_ERR("Failed to get sensor value");
		return;
	}
	TRACE_INF("Sensor driver: sensor_channel_get returned %d, %d", val.val1, val.val2);
#else
	TRACE_INF("doorstep_somedriver disabled in configuration");
#endif
}

} /* namespace */

int main(void)
{
	TRACE_INF("Main function started");
	TRACE_INF("Calling driver API");
	call_driver_api();
	TRACE_INF("Main function finished");
	return 0;
}
