/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Sample device-independent subsystem API for the course doorstep driver.
 *
 * Applications use doorstep_do_this() and doorstep_do_that() with a struct device;
 * each driver supplies a doorstep_driver_api via DEVICE_API(doorstep, ...).
 *
 * The reference doorstep_somedriver implementation toggles the board "led0" GPIO:
 * do_this() returns 0 or a negative errno from gpio_pin_toggle_dt; do_that() has
 * no return channel (void), so GPIO errors there are handled inside the driver only.
 */

#ifndef ZEPHYR_INCLUDE_DRIVERS_DOORSTEP_H_
#define ZEPHYR_INCLUDE_DRIVERS_DOORSTEP_H_

#include <errno.h>

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @typedef doorstep_api_do_this_t
 * @brief Driver hook for the do_this operation (driver-defined behavior).
 *
 * @param dev Device instance passed by the subsystem.
 * @param foo Opaque application parameter (forwarded from doorstep_do_this()).
 * @param bar Opaque application parameter (forwarded from doorstep_do_this()).
 *
 * @retval 0 Success.
 * @retval -errno Failure (e.g. the sample driver returns gpio_pin_toggle_dt() errors).
 */
typedef int (*doorstep_api_do_this_t)(const struct device *dev, int foo, int bar);

/**
 * @typedef doorstep_api_do_that_t
 * @brief Driver hook for the do_that operation (void return; no errno to caller).
 *
 * @param dev Device instance passed by the subsystem.
 * @param baz Opaque context from the application; may be NULL (sample driver logs it).
 */
typedef void (*doorstep_api_do_that_t)(const struct device *dev, void *baz);

/**
 * @brief Table of driver-specific operations for the "doorstep" class.
 *
 * Implementations are placed in iterable sections via DEVICE_API(doorstep, name).
 */
__subsystem struct doorstep_driver_api {
	doorstep_api_do_this_t do_this;
	doorstep_api_do_that_t do_that;
};

/**
 * @brief Call the driver's do_this handler.
 *
 * @param dev Device pointer (e.g. DEVICE_DT_GET(DT_NODELABEL(...))).
 * @param foo Forwarded to the driver hook.
 * @param bar Forwarded to the driver hook.
 *
 * @retval 0 Driver succeeded.
 * @retval -ENOSYS No do_this operation registered for this device.
 * @retval -errno Otherwise the value returned by the driver (see doorstep_api_do_this_t).
 */
static inline int doorstep_do_this(const struct device *dev, int foo, int bar)
{
	const struct doorstep_driver_api *api = DEVICE_API_GET(doorstep, dev);

	if (api->do_this == NULL) {
		return -ENOSYS;
	}

	return api->do_this(dev, foo, bar);
}

/**
 * @brief Call the driver's do_that handler (no return value).
 *
 * If the hook is missing, this is a no-op. If present, errors from driver I/O cannot be
 * reported here because the API is void; the sample driver emits a warning and returns
 * early when gpio_pin_toggle_dt() fails.
 *
 * @param dev Device pointer.
 * @param baz Forwarded to the driver hook; may be NULL.
 */
static inline void doorstep_do_that(const struct device *dev, void *baz)
{
	const struct doorstep_driver_api *api = DEVICE_API_GET(doorstep, dev);

	if (api->do_that == NULL) {
		return;
	}

	api->do_that(dev, baz);
}

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_DRIVERS_DOORSTEP_H_ */
