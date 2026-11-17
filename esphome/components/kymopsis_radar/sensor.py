import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import DEVICE_CLASS_DISTANCE, STATE_CLASS_MEASUREMENT, UNIT_METER

from . import CONF_KYMOPSIS_RADAR_ID, KymopsisRadar

CONF_PEOPLE = "people"
CONF_NEAREST_DISTANCE = "nearest_distance"
CONF_BREATHING_RATE = "breathing_rate"
CONF_HEART_RATE = "heart_rate"
UNIT_PER_MINUTE = "/min"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_KYMOPSIS_RADAR_ID): cv.use_id(KymopsisRadar),
        cv.Optional(CONF_PEOPLE): sensor.sensor_schema(icon="mdi:account-multiple", accuracy_decimals=0,
                                                       state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_NEAREST_DISTANCE): sensor.sensor_schema(unit_of_measurement=UNIT_METER, accuracy_decimals=2,
                                                                 device_class=DEVICE_CLASS_DISTANCE,
                                                                 state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_BREATHING_RATE): sensor.sensor_schema(unit_of_measurement=UNIT_PER_MINUTE, icon="mdi:lungs",
                                                               accuracy_decimals=0, state_class=STATE_CLASS_MEASUREMENT),
        cv.Optional(CONF_HEART_RATE): sensor.sensor_schema(unit_of_measurement=UNIT_PER_MINUTE, icon="mdi:heart-pulse",
                                                           accuracy_decimals=0, state_class=STATE_CLASS_MEASUREMENT),
    }
)


async def to_code(config):
    radar = await cg.get_variable(config[CONF_KYMOPSIS_RADAR_ID])
    for key, setter in ((CONF_PEOPLE, "set_people_sensor"), (CONF_NEAREST_DISTANCE, "set_nearest_sensor"),
                        (CONF_BREATHING_RATE, "set_breathing_sensor"), (CONF_HEART_RATE, "set_heart_sensor")):
        if key in config:
            cg.add(getattr(radar, setter)(await sensor.new_sensor(config[key])))
