from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

from . import (
    PM5000SSensor,
    SENSOR_KEYS,
    CONF_ADDRESS,
    UNIT_PARTICLES_PER_LITER,
    pm5000s_ns,
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PM5000SSensor),
            cv.Optional(CONF_ADDRESS, default=0x28): cv.i2c_address,
            **{
                cv.Optional(sensor_key): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PARTICLES_PER_LITER,
                    icon="mdi:chemical-weapon",
                    accuracy_decimals=0,
                )
                for sensor_key in SENSOR_KEYS
            },
        }
    )
    .extend(cv.polling_component_schema("10s"))
    .extend(cv.requires_component("i2c"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await cg.register_component(var, config)

    for key, setter in SENSOR_KEYS.items():
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, setter)(sens))
