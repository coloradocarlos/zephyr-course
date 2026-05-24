#include <stdio.h>

#include <limits.h>

#include <zephyr/autoconf.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>

#if IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)
#if IS_ENABLED(CONFIG_SENSOR)
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#else
#include <zephyr/drivers/doorstep.h>
#endif
#if IS_ENABLED(CONFIG_SHELL)
#include <zephyr/shell/shell.h>
#endif
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

// Lesson 6 and 7: Task 1: Use the sensor driver API
#if IS_ENABLED(CONFIG_SENSOR)

void call_driver_api_with_sensor_driver(void)
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
	TRACE_INF("sensor driver disabled in configuration");
#endif
}

#else

// Lesson 6 and 7: Task 2: Use the custom extension API
void call_driver_api_with_custom_extension(void)
{
#if IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)
	TRACE_INF("doorstep_somedriver probe");
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));
	if (dev == NULL) {
		TRACE_ERR("Device not found");
		return;
	}

	int foo = 1;
	int bar = 2;
	int ret = doorstep_do_this(dev, foo, bar);
	if (ret < 0) {
		TRACE_ERR("doorstep_do_this failed: %d", ret);
		return;
	}
	TRACE_INF("doorstep_do_this returned %d", ret);

	// Sleep for half a second to allow the LED to visually toggle
	k_msleep(500);

	void *baz = NULL;
	doorstep_do_that(dev, baz);
	TRACE_INF("doorstep_do_that finished");
#else
	TRACE_INF("doorstep_somedriver disabled in configuration");
#endif
}

#endif

} /* namespace */

#if IS_ENABLED(CONFIG_SHELL) && IS_ENABLED(CONFIG_SENSOR) && IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)

extern "C" {

static int cmd_sensor_fetch(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));

	if (!device_is_ready(dev)) {
		shell_error(sh, "Sensor device not ready");
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(dev);

	if (ret < 0) {
		shell_error(sh, "sensor_sample_fetch failed: %d", ret);
	} else {
		shell_print(sh, "sensor_sample_fetch: %d", ret);
	}

	return ret;
}

static int cmd_sensor_read(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));

	if (!device_is_ready(dev)) {
		shell_error(sh, "Sensor device not ready");
		return -ENODEV;
	}

	struct sensor_value val;
	int ret = sensor_channel_get(dev, SENSOR_CHAN_ALL, &val);

	if (ret < 0) {
		shell_error(sh, "sensor_channel_get failed: %d", ret);
	} else {
		// TODO: val1 + val2*1e-6 is the Zephyr sensor_value decoding (integer part + micro fractional
		//       part); use sensor_value_to_float/double helpers in sensor.h instead of assuming a mantissa/exponent layout.
		shell_print(sh, "sensor_channel_get: ret=%d val1=%d val2=%d", ret, val.val1, val.val2);
	}

	return ret;
}

static int cmd_sensor_info(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));

	shell_print(sh, "name: %s", dev->name);
	shell_print(sh, "ready: %s", device_is_ready(dev) ? "true" : "false");
	return 0;
}

} /* extern "C" */

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor,
			     SHELL_CMD(fetch, NULL, "Fetch a new sensor sample", cmd_sensor_fetch),
			     SHELL_CMD(read, NULL, "Read sensor channel values", cmd_sensor_read),
			     SHELL_CMD(info, NULL, "Show sensor device name and ready state", cmd_sensor_info),
			     SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor, "Sensor commands", NULL);

#endif

#if IS_ENABLED(CONFIG_SHELL) && !IS_ENABLED(CONFIG_SENSOR) && IS_ENABLED(CONFIG_DOORSTEP_SOMEDRIVER)

/* SHELL_CMD_ARG mandatory count includes the subcommand token: set + foo + bar */
#define SENSOR_SHELL_SET_ARGC 3

extern "C" {

/* Last parameters for sensor get; default to zero until sensor set. */
static int sensor_shell_foo = 0;
static int sensor_shell_bar = 0;

static int cmd_sensor_get(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(doorstep_somedriver0));

	if (!device_is_ready(dev)) {
		shell_error(sh, "Doorstep device not ready");
		return -ENODEV;
	}

	const int foo = sensor_shell_foo;
	const int bar = sensor_shell_bar;
	int ret = doorstep_do_this(dev, foo, bar);

	if (ret < 0) {
		shell_error(sh, "doorstep_do_this failed: %d", ret);
	} else {
		shell_print(sh, "doorstep_do_this(foo=%d, bar=%d): %d", foo, bar, ret);
	}

	return ret;
}

/* argv: [0]=set, [1]=foo, [2]=bar — argc fixed by SHELL_CMD_ARG(..., SENSOR_SHELL_SET_ARGC, 0). */
static int cmd_sensor_set(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);

	int parse_err = 0;
	const long foo_l = shell_strtol(argv[1], 10, &parse_err);

	if (parse_err != 0) {
		shell_error(sh, "foo must be an integer");
		return parse_err;
	}

	parse_err = 0;
	const long bar_l = shell_strtol(argv[2], 10, &parse_err);

	if (parse_err != 0) {
		shell_error(sh, "bar must be an integer");
		return parse_err;
	}

	if (foo_l > INT_MAX || foo_l < INT_MIN || bar_l > INT_MAX || bar_l < INT_MIN) {
		shell_error(sh, "foo and bar must fit in int range");
		return -ERANGE;
	}

	const int foo = (int)foo_l;
	const int bar = (int)bar_l;

	sensor_shell_foo = foo;
	sensor_shell_bar = bar;

	shell_print(sh, "sensor set: success (foo=%d, bar=%d)", foo, bar);

	return 0;
}

} /* extern "C" */

SHELL_STATIC_SUBCMD_SET_CREATE(sub_sensor_doorstep,
			       SHELL_CMD(get, NULL, "Call doorstep_do_this with last foo/bar from sensor set", cmd_sensor_get),
			       SHELL_CMD_ARG(set, NULL, "Store integer foo and bar for sensor get", cmd_sensor_set,
					      SENSOR_SHELL_SET_ARGC, 0),
			       SHELL_SUBCMD_SET_END);

SHELL_CMD_REGISTER(sensor, &sub_sensor_doorstep, "Doorstep sensor commands", NULL);

#endif

int main(void)
{
	TRACE_INF("Main function started");
	TRACE_INF("Calling driver API");

// Lesson 6 and 7: Task 1: Use the sensor driver API
// Lesson 6 and 7: Task 2: Use the custom extension API
#if IS_ENABLED(CONFIG_SENSOR)
	call_driver_api_with_sensor_driver(); // Task 1
#else
	call_driver_api_with_custom_extension(); // Task 2
#endif
	TRACE_INF("Main function finished");
	return 0;
}
