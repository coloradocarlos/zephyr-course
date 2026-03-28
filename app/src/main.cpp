#include <stdio.h>
#include <string.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/logging/log_output.h>
#include <openamp/open_amp.h>

#define SLEEP_TIME_MS 1000

/* Define the log buffer size and storage */
#define TRACE_BUF_SIZE 1024
char system_log_buf[TRACE_BUF_SIZE] __attribute__((section(".trace_buffer")));
static uint32_t log_buf_index = 0;

/* Resource table structure */
struct my_resource_table {
    struct resource_table base;
    uint32_t offset[1]; /* One entry for the trace */

    struct fw_rsc_trace trace;
} __packed;

/* Define the table in the section Linux expects */
__attribute__((section(".resource_table")))
const struct my_resource_table board_res_table = {
    .base = {
        .ver = 1,
        .num = 1,
        .reserved = {0, 0},
    },
    .offset = {
        offsetof(struct my_resource_table, trace),
    },
    .trace = {
        .type = RSC_TRACE,
        .da = (uint32_t)&system_log_buf, /* Device Address */
        .len = TRACE_BUF_SIZE,
        .reserved = 0,
        .name = "trace0",
    },
};

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
#define LED_NODE DT_ALIAS(led0)
#define LED_NODE_2 DT_ALIAS(led1)

// The OSD32MP1 Breakout Board has two LEDs: Green and Red.
// The Green LED is connected to the GPIOI pin 9 and the Red LED is connected to the GPIOI pin 8.
// Green LED configuration:
static const struct gpio_dt_spec led_green = GPIO_DT_SPEC_GET(LED_NODE, gpios);
// Red LED configuration:
static const struct gpio_dt_spec led_red = GPIO_DT_SPEC_GET(LED_NODE_2, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

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
        TRACE_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);

        // Red LED toggle the GPIO
        if (gpio_pin_toggle_dt(&led_red) < 0) return 0;
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}
