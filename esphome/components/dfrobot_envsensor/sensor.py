import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID, CONF_TEMPERATURE, CONF_HUMIDITY, CONF_PRESSURE,
    UNIT_CELSIUS, UNIT_PERCENT, UNIT_HECTOPASCAL, UNIT_LUX, UNIT_EMPTY, UNIT_METER,
    DEVICE_CLASS_TEMPERATURE, DEVICE_CLASS_HUMIDITY, DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_ILLUMINANCE, DEVICE_CLASS_EMPTY, DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT
)

CONF_LIGHT = "light"
CONF_UV_INDEX = "uv_index"
CONF_ELEVATION = "elevation"

AUTO_LOAD = ["sensor"]

dfrobot_envsensor_ns = cg.esphome_ns.namespace("dfrobot_envsensor")
DFRobotEnvSensor = dfrobot_envsensor_ns.class_(
    "DFRobotEnvSensor", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotEnvSensor),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_CELSIUS,
        icon="mdi:thermometer",
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_TEMPERATURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
        unit_of_measurement=UNIT_PERCENT,
        icon="mdi:water-percent",
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_HUMIDITY,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_PRESSURE): sensor.sensor_schema(
        unit_of_measurement=UNIT_HECTOPASCAL,
        icon="mdi:gauge",
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_PRESSURE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_LIGHT): sensor.sensor_schema(
        unit_of_measurement=UNIT_LUX,
        icon="mdi:brightness-5",
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_ILLUMINANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_UV_INDEX): sensor.sensor_schema(
        unit_of_measurement=UNIT_EMPTY,
        icon="mdi:weather-sunny",
        accuracy_decimals=2,
        device_class=DEVICE_CLASS_EMPTY,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Optional(CONF_ELEVATION): sensor.sensor_schema(
        unit_of_measurement=UNIT_METER,
        icon="mdi:image-filter-hdr",
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_DISTANCE,
        state_class=STATE_CLASS_MEASUREMENT,
    ),
    cv.Required("update_interval"): cv.update_interval,
}).extend(i2c.i2c_device_schema(None))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))

    if CONF_PRESSURE in config:
        sens = await sensor.new_sensor(config[CONF_PRESSURE])
        cg.add(var.set_pressure_sensor(sens))

    if CONF_LIGHT in config:
        sens = await sensor.new_sensor(config[CONF_LIGHT])
        cg.add(var.set_light_sensor(sens))

    if CONF_UV_INDEX in config:
        sens = await sensor.new_sensor(config[CONF_UV_INDEX])
        cg.add(var.set_uv_index_sensor(sens))

    if CONF_ELEVATION in config:
        sens = await sensor.new_sensor(config[CONF_ELEVATION])
        cg.add(var.set_elevation_sensor(sens))

    cg.add(var.set_update_interval(config["update_interval"]))
