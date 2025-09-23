import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c
from esphome.const import CONF_ID, CONF_ADDRESS

CODEOWNERS = ["@Lafayette"]
MULTI_CONF = True
DEPENDENCIES = ["i2c"]

CONF_PM5000S_ID = "pm5000s_id"
CONF_ADDRESS = "address"

pm5000s_ns = cg.esphome_ns.namespace("pm5000s")
PM5000SSensor = pm5000s_ns.class_(
    "PM5000SSensor", cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PM5000SSensor),
            cv.Optional(CONF_ADDRESS, default=0x28): cv.uint8_t,
        }
    )
    .extend(i2c.i2c_device_schema)
    .extend(cv.polling_component_schema("5s"))
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_ADDRESS])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    for sensor_key in [
        "particles_03um",
        "particles_05um",
        "particles_10um",
        "particles_25um",
        "particles_50um",
        "particles_100um",
    ]:
        if sensor_key in config:
            sensor_var = await cg.get_variable(config[sensor_key])
            cg.add(var.set_sensor(sensor_key, sensor_var))
