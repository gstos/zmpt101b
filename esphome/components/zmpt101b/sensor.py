import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_PIN,
    CONF_SENSITIVITY,
    CONF_FREQUENCY,
    UNIT_VOLT,
    ICON_FLASH,
    )

zmpt101b_ns = cg.esphome_ns.namespace("zmpt101b")
ZMPT101BSensor = zmpt101b_ns.class_("ZMPT101BSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_VOLT,
    icon=ICON_FLASH,
    accuracy_decimals=1,
    ).extend({
    cv.GenerateID(): cv.declare_id(ZMPT101BSensor),
    cv.Required(CONF_PIN): cv.int_,
    cv.Optional(CONF_SENSITIVITY, default=1.0): cv.float_,
    cv.Optional(CONF_FREQUENCY, default=60): cv.int_,
    }).extend(cv.polling_component_schema("60s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PIN])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    if (sensitivity := config.get(CONF_SENSITIVITY)) is not None:
        cg.add(var.set_sensitivity(sensitivity))

    if (frequency := config.get(CONF_FREQUENCY)) is not None:
        cg.add(var.set_frequency(frequency))
