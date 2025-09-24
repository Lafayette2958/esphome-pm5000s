import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import CONF_ID, CONF_ADDRESS

from . import (
    PM5000SSensor,
    SENSOR_KEYS,
    UNIT_PARTICLES_PER_LITER,
)

DEPENDENCIES = ["i2c"]
AUTO_LOAD = ["i2c"]

CONFIG_SCHEMA = (
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
    .extend(i2c.i2c_device_schema(default_address=0x28))
    .extend(cv.polling_component_schema("10s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for key, setter in SENSOR_KEYS.items():
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, setter)(sens))
