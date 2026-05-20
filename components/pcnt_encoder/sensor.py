import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor

pcnt_ns = cg.esphome_ns.namespace("pcnt_encoder")
PCNTEncoder = pcnt_ns.class_("PCNTEncoder", cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement="m/min",
    accuracy_decimals=2,
).extend({
    cv.Required("pin_a"): cv.gpio_pin,
    cv.Required("pin_b"): cv.gpio_pin,
    cv.Required("meters_per_pulse"): cv.float_,
    cv.Optional("distance"): sensor.sensor_schema(
        unit_of_measurement="m",
        accuracy_decimals=3,
    ),
}).extend(cv.polling_component_schema("200ms"))


def to_code(config):
    var = cg.new_Pvariable(
        config["id"],
        config["pin_a"],
        config["pin_b"],
        config["meters_per_pulse"],
    )

    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    if "distance" in config:
        sens = yield sensor.new_sensor(config["distance"])
        cg.add(var.set_distance_sensor(sens))
