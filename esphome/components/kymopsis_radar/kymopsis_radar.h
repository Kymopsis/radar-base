#pragma once

#include <cmath>
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif

namespace esphome {
namespace kymopsis_radar {

// Frames from the radar app on its UART0 (8-byte header: 0x01, id, len, type big-endian, ~XOR; payload; ~XOR).
class KymopsisRadar : public Component, public uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_reset_pin(GPIOPin *p) { reset_pin_ = p; }
  void set_boot_high_pin(GPIOPin *p) { boot_high_pin_ = p; }
  void set_boot_low_pin(GPIOPin *p) { boot_low_pin_ = p; }
  void set_mount(int m) { mount_ = m; }
  void set_report_timeout(uint32_t ms) { timeout_ms_ = ms; }
#ifdef USE_BINARY_SENSOR
  void set_occupancy_sensor(binary_sensor::BinarySensor *s) { occupancy_ = s; }
  void set_fall_sensor(binary_sensor::BinarySensor *s) { fall_ = s; }
  void set_area_sensor(int i, binary_sensor::BinarySensor *s) { if (i >= 0 && i < 4) area_[i] = s; }
#endif
#ifdef USE_SENSOR
  void set_people_sensor(sensor::Sensor *s) { people_ = s; }
  void set_nearest_sensor(sensor::Sensor *s) { nearest_ = s; }
  void set_breathing_sensor(sensor::Sensor *s) { breathing_ = s; }
  void set_heart_sensor(sensor::Sensor *s) { heart_ = s; }
#endif

  // Send a frame to the radar app (e.g. type 0x0201 with a uint32 command).
  void send_frame(uint16_t type, const uint8_t *data, uint16_t len);

 protected:
  void on_byte_(uint8_t c);
  void on_frame_(uint16_t type, const uint8_t *p, uint16_t len);
  void on_targets_(const uint8_t *p, uint16_t len);

  GPIOPin *reset_pin_{nullptr}, *boot_high_pin_{nullptr}, *boot_low_pin_{nullptr};
  int mount_{-1};
  uint32_t timeout_ms_{3000};
  uint32_t last_targets_ms_{0};
  bool cleared_{true};
  uint16_t tx_id_{0x8001};

  // receiver
  uint8_t st_{0};
  uint16_t got_{0}, len_{0}, type_{0};
  uint8_t head_[8]{};
  uint8_t pl_[1024]{};
  bool trunc_{false};

#ifdef USE_BINARY_SENSOR
  binary_sensor::BinarySensor *occupancy_{nullptr}, *fall_{nullptr}, *area_[4]{};
#endif
#ifdef USE_SENSOR
  sensor::Sensor *people_{nullptr}, *nearest_{nullptr}, *breathing_{nullptr}, *heart_{nullptr};
#endif
};

}  // namespace kymopsis_radar
}  // namespace esphome
