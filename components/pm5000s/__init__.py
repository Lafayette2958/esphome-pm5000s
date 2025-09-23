import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    CONF_ID,
    ICON_CHEMICAL_WEAPON,
    DEVICE_CLASS_EMPTY,  # No specific device class for particle count
)

DEPENDENCIES = ["i2c"]
CODEOWNERS = ["@Lafayette"]
MULTI_CONF = True

CONF_ADDRESS = "address"

pm5000s_ns = cg.esphome_ns.namespace("pm5000s")
PM5000SSensor = pm5000s_ns.class_("PM5000SSensor", cg.PollingComponent, i2c.I2CDevice)

SENSOR_KEYS = {
    "particles_03um": "set_particles_03um_sensor",
    "particles_05um": "set_particles_05um_sensor",
    "particles_10um": "set_particles_10um_sensor",
    "particles_25um": "set_particles_25um_sensor",
    "particles_50um": "set_particles_50um_sensor",
    "particles_100um": "set_particles_100um_sensor",
}

UNIT_PARTICLES_PER_LITER = "pcs/L"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PM5000SSensor),
            cv.Optional(CONF_ADDRESS, default=0x28): cv.i2c_address,
            **{
                cv.Optional(sensor_key): sensor.sensor_schema(
                    unit_of_measurement=UNIT_PARTICLES_PER_LITER,
                    icon=ICON_CHEMICAL_WEAPON,
                    accuracy_decimals=0,
                    device_class=DEVICE_CLASS_EMPTY,
                )
                for sensor_key in SENSOR_KEYS
            },
        }
    )
    .extend(i2c.i2c_device_schema(default_address=0x28))  
    .extend(cv.polling_component_schema("5s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for key, setter in SENSOR_KEYS.items():
        if key in config:
            sens = await sensor.new_sensor(config[key])
            cg.add(getattr(var, setter)(sens))
