import esphome.config_validation as cv
from esphome import automation
from esphome.const import CONF_ID
import esphome.codegen as cg

dfrobot_envsensor_ns = cg.esphome_ns.namespace('dfrobot_envsensor')
DFRobotEnvSensor = dfrobot_envsensor_ns.class_('DFRobotEnvSensor', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DFRobotEnvSensor),
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
