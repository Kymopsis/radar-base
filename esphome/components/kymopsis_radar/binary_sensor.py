import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import DEVICE_CLASS_OCCUPANCY, DEVICE_CLASS_SAFETY

from . import CONF_KYMOPSIS_RADAR_ID, KymopsisRadar

CONF_OCCUPANCY = "occupancy"
CONF_FALL = "fall"
AREAS = [f"area_{i}" for i in range(1, 5)]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KYMOPSIS_RADAR_ID): cv.use_id(KymopsisRadar),
        cv.Optional(CONF_OCCUPANCY): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_OCCUPANCY),
        cv.Optional(CONF_FALL): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_SAFETY,
                                                                   icon="mdi:human-handsdown"),
        **{cv.Optional(a): binary_sensor.binary_sensor_schema(device_class=DEVICE_CLASS_OCCUPANCY) for a in AREAS},
    }
)


async def to_code(config):
    radar = await cg.get_variable(config[CONF_KYMOPSIS_RADAR_ID])
    if CONF_OCCUPANCY in config:
        cg.add(radar.set_occupancy_sensor(await binary_sensor.new_binary_sensor(config[CONF_OCCUPANCY])))
    if CONF_FALL in config:
        cg.add(radar.set_fall_sensor(await binary_sensor.new_binary_sensor(config[CONF_FALL])))
    for i, a in enumerate(AREAS):
        if a in config:
            cg.add(radar.set_area_sensor(i, await binary_sensor.new_binary_sensor(config[a])))
