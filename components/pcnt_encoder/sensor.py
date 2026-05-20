import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_PIN
import esphome.pins as pins

pcnt_ns = cg.esphome_ns.namespace("pcnt_encoder")
PCNTEncoder = pcnt_ns.class_("PCNTEncoder", cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement="m/min",
    accuracy_decimals=2,
).extend({
    cv.Required("pin_a"): pins.gpio_output_pin_schema,
    cv.Required("pin_b"): pins.gpio_output_pin_schema,
    cv.Required("meters_per_pulse"): cv.float_,
    cv.Optional("distance"): sensor.sensor_schema(
        unit_of_measurement="m",
        accuracy_decimals=3,
    ),
}).extend(cv.polling_component_schema("200ms"))


def to_code(config):
    pin_a = yield cg.gpio_pin_expression(config["pin_a"])
    pin_b = yield cg.gpio_pin_expression(config["pin_b"])

    var = cg.new_Pvariable(
        config["id"],
        pin_a,
        pin_b,
        config["meters_per_pulse"],
    )

    yield cg.register_component(var, config)
    yield sensor.register_sensor(var, config)

    if "distance" in config:
        sens = yield sensor.new_sensor(config["distance"])
        cg.add(var.set_distance_sensor(sens))
