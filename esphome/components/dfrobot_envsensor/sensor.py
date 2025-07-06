import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    CONF_ADDRESS,
    CONF_UPDATE_INTERVAL,
    CONF_TEMPERATURE,
    CONF_HUMIDITY,
    CONF_PRESSURE,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    UNIT_HECTOPASCAL,
    UNIT_LUX,
    UNIT_METER,
    UNIT_EMPTY,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_ILLUMINANCE,
    DEVICE_CLASS_DISTANCE,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT,
)

# Custom config keys for this component
CONF_LIGHT = "light"
CONF_UV_INDEX = "uv_index"
CONF_ELEVATION = "elevation"
CONF_UV_GAIN = "uv_gain"
CONF_UV_RESOLUTION = "uv_resolution"
CONF_UV_RATE = "uv_rate"
CONF_LIGHT_GAIN = "light_gain"
CONF_LIGHT_INTEGRATION_TIME = "light_integration_time"

# Load the C++ namespace
[dfrobot_envsensor_ns] = cg.esphome_ns.namespace("dfrobot_envsensor") if False else (cg.esphome_ns.namespace("dfrobot_envsensor"),)
DFRobotEnvSensor = dfrobot_envsensor_ns.class_(
    "DFRobotEnvSensor", cg.PollingComponent, i2c.I2CDevice
)

# Schema definition
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotEnvSensor),
    cv.Optional(CONF_ADDRESS, default=0x22): cv.i2c_address,
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
    # Optional UV sensor configuration (0-255)
    cv.Optional(CONF_UV_GAIN): cv.int_range(min=0, max=255),
    cv.Optional(CONF_UV_RESOLUTION): cv.int_range(min=0, max=255),
    cv.Optional(CONF_UV_RATE): cv.int_range(min=0, max=255),
    # New light sensor calibration parameters
    cv.Optional(CONF_LIGHT_GAIN, default=1.0): cv.float_range(min=0.125, max=2.0),
    cv.Optional(CONF_LIGHT_INTEGRATION_TIME, default=100): cv.int_range(min=25, max=800),
    cv.Required(CONF_UPDATE_INTERVAL): cv.update_interval,
}).extend(i2c.i2c_device_schema(None))

# Conversion to code
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
    # Pass through light calibration
    cg.add(var.set_light_gain(config[CONF_LIGHT_GAIN]))
    cg.add(var.set_light_integration_time(config[CONF_LIGHT_INTEGRATION_TIME]))

    if CONF_UV_INDEX in config:
        sens = await sensor.new_sensor(config[CONF_UV_INDEX])
        cg.add(var.set_uv_index_sensor(sens))

    if CONF_ELEVATION in config:
        sens = await sensor.new_sensor(config[CONF_ELEVATION])
        cg.add(var.set_elevation_sensor(sens))

    # Pass through any UV configuration settings
    if CONF_UV_GAIN in config:
        cg.add(var.set_uv_gain(config[CONF_UV_GAIN]))
    if CONF_UV_RESOLUTION in config:
        cg.add(var.set_uv_resolution(config[CONF_UV_RESOLUTION]))
    if CONF_UV_RATE in config:
        cg.add(var.set_uv_rate(config[CONF_UV_RATE]))

    cg.add(var.set_update_interval(config[CONF_UPDATE_INTERVAL]))
