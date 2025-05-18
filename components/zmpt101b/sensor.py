import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_NAME, CONF_PIN, CONF_SENSITIVITY, CONF_FREQUENCY, ICON_FLASH, UNIT_VOLT

DEPENDENCIES = []
AUTO_LOAD = []

zmpt101b_ns = cg.esphome_ns.namespace('zmpt101b')
ZMPT101BSensor = zmpt101b_ns.class_('ZMPT101BSensor', sensor.Sensor, cg.PollingComponent)

CONF_SAMPLING_PIN = CONF_PIN

CONFIG_SCHEMA = sensor.sensor_schema(UNIT_VOLT, ICON_FLASH, 1).extend({
    cv.GenerateID(): cv.declare_id(ZMPT101BSensor),
    cv.Required(CONF_PIN): cv.int_,
    cv.Optional(CONF_SENSITIVITY, default=1.0): cv.float_,
    cv.Optional(CONF_FREQUENCY, default=50): cv.int_,
    cv.Optional("update_interval", default="60s"): cv.update_interval,
    }).extend(cv.polling_component_schema("60s"))

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PIN], config[CONF_SENSITIVITY], config[CONF_FREQUENCY])
    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)