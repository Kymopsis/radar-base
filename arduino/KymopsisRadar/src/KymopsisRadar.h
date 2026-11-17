// Kymopsis Radar Base: parser for the result frames of the 60 GHz radar apps (radar UART0, 115200 8N1).
// Works on any Arduino core; no dynamic memory. Same values as the Radar Base web app, MQTT and ESPHome.
#pragma once
#include <Arduino.h>

struct KymopsisTarget {
  float x, y, z;      // metres, radar coordinates
  int32_t id;
};

class KymopsisRadar {
 public:
  static const uint8_t MAX_TARGETS = 16;

  explicit KymopsisRadar(Stream &uart) : uart_(uart) {}

  // Call often (from loop()). Returns true when at least one complete frame was decoded.
  bool update();

  // Ask the presence app for a mounting: true = side (wall), false = ceiling.
  void setMount(bool side);
  // Send any command frame to the radar app.
  void sendFrame(uint16_t type, const uint8_t *data, uint16_t len);

  // Latest results (NAN / -1 when not reported by the running app).
  bool occupied() const { return people_ > 0 || human_ == 1; }
  int people() const { return people_; }
  float nearest() const { return nearest_; }                 // metres
  const KymopsisTarget &target(uint8_t i) const { return targets_[i < MAX_TARGETS ? i : 0]; }
  bool area(uint8_t i) const { return i < 4 && (areas_ >> i) & 1; }  // areas 0..3
  int fall() const { return fall_; }                         // -1 unknown, 0 normal, 1 fall
  float breathing() const { return breathing_; }             // breaths per minute
  float heartRate() const { return heart_; }                 // beats per minute
  uint32_t lastFrameMs() const { return last_ms_; }

  // Optional: called for every valid frame (type, payload, length).
  void onFrame(void (*cb)(uint16_t, const uint8_t *, uint16_t)) { cb_ = cb; }

 private:
  void byte_(uint8_t c);
  void frame_(uint16_t type, const uint8_t *p, uint16_t len);

  Stream &uart_;
  uint8_t st_ = 0, head_[8] = {0}, pl_[512] = {0};
  uint16_t got_ = 0, len_ = 0, type_ = 0, tx_id_ = 0x8001;
  bool decoded_ = false;
  void (*cb_)(uint16_t, const uint8_t *, uint16_t) = nullptr;

  KymopsisTarget targets_[MAX_TARGETS] = {};
  int people_ = 0, human_ = -1, fall_ = -1;
  uint8_t areas_ = 0;
  float nearest_ = NAN, breathing_ = NAN, heart_ = NAN;
  uint32_t last_ms_ = 0;
};
