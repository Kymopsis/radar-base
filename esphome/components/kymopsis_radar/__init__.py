"""Kymopsis Radar Base: ESPHome external component.

Reads the result frames of the radar app running on the 60 GHz radar module (its UART0, 115200 8N1) and turns
them into ESPHome sensors: occupancy, people, nearest distance, four areas, fall, breathing and heart rate.
The radar app must start from the module's own flash (dev-kit modules ship that way).
"""
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@Kymopsis"]
DEPENDENCIES = ["uart"]
MULTI_CONF = True

CONF_KYMOPSIS_RADAR_ID = "kymopsis_radar_id"
CONF_RESET_PIN = "reset_pin"
CONF_BOOT_HIGH_PIN = "boot_high_pin"
CONF_BOOT_LOW_PIN = "boot_low_pin"
CONF_MOUNT = "mount"
CONF_TIMEOUT = "timeout"

kymopsis_radar_ns = cg.esphome_ns.namespace("kymopsis_radar")
KymopsisRadar = kymopsis_radar_ns.class_("KymopsisRadar", cg.Component, uart.UARTDevice)

MOUNTS = {"ceiling": 0, "side": 1, "keep": -1}

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(KymopsisRadar),
            cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_BOOT_HIGH_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_BOOT_LOW_PIN): pins.gpio_output_pin_schema,
            cv.Optional(CONF_MOUNT, default="keep"): cv.one_of(*MOUNTS, lower=True),
            cv.Optional(CONF_TIMEOUT, default="3s"): cv.positive_time_period_milliseconds,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "kymopsis_radar", baud_rate=115200, require_tx=False, require_rx=True
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
    for key, setter in ((CONF_RESET_PIN, "set_reset_pin"), (CONF_BOOT_HIGH_PIN, "set_boot_high_pin"),
                        (CONF_BOOT_LOW_PIN, "set_boot_low_pin")):
        if key in config:
            pin = await cg.gpio_pin_expression(config[key])
            cg.add(getattr(var, setter)(pin))
    cg.add(var.set_mount(MOUNTS[config[CONF_MOUNT]]))
    cg.add(var.set_report_timeout(config[CONF_TIMEOUT]))
