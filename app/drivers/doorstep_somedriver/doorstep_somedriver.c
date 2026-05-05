#define DT_DRV_COMPAT doorstep_somedriver

#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(doorstep_somedriver, LOG_LEVEL_INF);

static int sensor_sample_fetch_impl(const struct device *dev, enum sensor_channel chan)
{
	LOG_INF("sensor_sample_fetch_impl %d", chan);
	return 0;
}

static int sensor_channel_get_impl(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	LOG_INF("sensor_channel_get_impl %d", chan);
	val->val1 = 100;	// TODO: Replace with actual sensor value
	val->val2 = 200;	// TODO: Replace with actual sensor value
	return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
	.sample_fetch = sensor_sample_fetch_impl,
	.channel_get = sensor_channel_get_impl,
};

static int init_my_impl(const struct device *dev)
{
	LOG_INF("init_my_impl");
	return 0;
}

DEVICE_DT_INST_DEFINE(0, init_my_impl, NULL, NULL, NULL, POST_KERNEL,
		      CONFIG_SENSOR_INIT_PRIORITY, &api_iomico_lecture);
