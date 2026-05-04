#define DT_DRV_COMPAT doorstep_somedriver

#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(doorstep_somedriver, LOG_LEVEL_INF);

static int channel_get_my_impl(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	LOG_INF("channel_get_my_impl %d", chan);
	return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
	.channel_get = channel_get_my_impl,
};

static int init_my_impl(const struct device *dev)
{
	LOG_INF("init_my_impl");
	return 0;
}

DEVICE_DT_INST_DEFINE(0, init_my_impl, NULL, NULL, NULL, POST_KERNEL,
		      CONFIG_SENSOR_INIT_PRIORITY, &api_iomico_lecture);
