#include "kymopsis_radar.h"
#include <cstring>
#include "esphome/core/log.h"

namespace esphome {
namespace kymopsis_radar {

static const char *const TAG = "kymopsis_radar";

// Result frames of the radar apps (same as the Radar Base firmware, see SCHEMA.md):
//   presence: 0x0A04 targets (int32 n, n x {float x, y, z; int32 doppler; int32 id}), 0x0A0A areas (4 x uint32)
//   fall:     0x0E02 uint8 is_fall, 0x0F09 uint8 is_human
//   vital:    0x0A14 float breathing /min, 0x0A15 float heart rate /min
static float rdf(const uint8_t *p) { float f; memcpy(&f, p, 4); return f; }
static int32_t rdi(const uint8_t *p) { int32_t v; memcpy(&v, p, 4); return v; }

void KymopsisRadar::setup() {
  // Start the radar app from the module's own flash: boot pins to "flash", then a reset pulse.
  if (this->boot_high_pin_ != nullptr) { this->boot_high_pin_->setup(); this->boot_high_pin_->digital_write(true); }
  if (this->boot_low_pin_ != nullptr) { this->boot_low_pin_->setup(); this->boot_low_pin_->digital_write(false); }
  if (this->reset_pin_ != nullptr) {
    this->reset_pin_->setup();
    this->reset_pin_->digital_write(false);
    delay(20);
    this->reset_pin_->digital_write(true);
  }
  this->last_targets_ms_ = millis();
  if (this->mount_ >= 0) {
    // presence app: 0x13 = ceiling, 0x14 = side; sent again a moment later in case the app was still starting
    this->set_timeout("mount", 1500, [this]() {
      uint32_t q = this->mount_ == 1 ? 0x14 : 0x13;
      this->send_frame(0x0201, reinterpret_cast<const uint8_t *>(&q), 4);
    });
  }
}

void KymopsisRadar::dump_config() {
  ESP_LOGCONFIG(TAG, "Kymopsis Radar Base (radar app results over UART)");
  LOG_PIN("  Reset pin: ", this->reset_pin_);
  LOG_PIN("  Boot high pin: ", this->boot_high_pin_);
  LOG_PIN("  Boot low pin: ", this->boot_low_pin_);
  ESP_LOGCONFIG(TAG, "  Mount: %s", this->mount_ < 0 ? "keep" : this->mount_ ? "side" : "ceiling");
  ESP_LOGCONFIG(TAG, "  Timeout: %u ms", (unsigned) this->timeout_ms_);
}

void KymopsisRadar::send_frame(uint16_t type, const uint8_t *data, uint16_t len) {
  uint8_t h[8] = {0x01, (uint8_t)(this->tx_id_ >> 8), (uint8_t) this->tx_id_, (uint8_t)(len >> 8), (uint8_t) len,
                  (uint8_t)(type >> 8), (uint8_t) type, 0};
  uint8_t x = 0;
  for (int i = 0; i < 7; i++) x ^= h[i];
  h[7] = ~x;
  this->write_array(h, 8);
  if (len) {
    x = 0;
    for (int i = 0; i < len; i++) x ^= data[i];
    this->write_array(data, len);
    this->write_byte(~x);
  }
  this->tx_id_ = this->tx_id_ == 0xFFFF ? 0x8001 : this->tx_id_ + 1;
}

void KymopsisRadar::loop() {
  uint8_t c;
  int n = 0;
  while (this->available() && n++ < 512 && this->read_byte(&c)) this->on_byte_(c);
  // nobody reported for a while: clear the presence values once
  if (!this->cleared_ && millis() - this->last_targets_ms_ > this->timeout_ms_) {
    this->cleared_ = true;
#ifdef USE_BINARY_SENSOR
    if (this->occupancy_ != nullptr) this->occupancy_->publish_state(false);
#endif
#ifdef USE_SENSOR
    if (this->people_ != nullptr) this->people_->publish_state(0);
    if (this->nearest_ != nullptr) this->nearest_->publish_state(NAN);
#endif
  }
}

void KymopsisRadar::on_byte_(uint8_t c) {
  switch (this->st_) {
    case 0:
      if (c == 0x01) { this->head_[0] = c; this->got_ = 1; this->st_ = 1; }
      break;
    case 1:
      this->head_[this->got_++] = c;
      if (this->got_ == 8) {
        uint8_t x = 0;
        for (int i = 0; i < 7; i++) x ^= this->head_[i];
        if ((uint8_t) ~x != this->head_[7]) { this->st_ = 0; break; }
        this->len_ = this->head_[3] << 8 | this->head_[4];
        this->type_ = this->head_[5] << 8 | this->head_[6];
        this->got_ = 0;
        this->trunc_ = this->len_ > sizeof(this->pl_);
        if (this->len_ == 0) { this->on_frame_(this->type_, nullptr, 0); this->st_ = 0; }
        else this->st_ = 2;
      }
      break;
    case 2:
      if (this->got_ < this->len_) {
        if (this->got_ < sizeof(this->pl_)) this->pl_[this->got_] = c;
        this->got_++;
        break;
      }
      // checksum byte
      this->st_ = 0;
      if (this->trunc_) break;  // long frames (point clouds) are not used here
      {
        uint8_t x = 0;
        for (int i = 0; i < this->len_; i++) x ^= this->pl_[i];
        if ((uint8_t) ~x != c) { ESP_LOGV(TAG, "checksum error, type 0x%04X", this->type_); break; }
      }
      this->on_frame_(this->type_, this->pl_, this->len_);
      break;
  }
}

void KymopsisRadar::on_targets_(const uint8_t *p, uint16_t len) {
  if (len < 4) return;
  int32_t n = rdi(p);
  int32_t max = (len - 4) / 20;
  if (n < 0) n = 0;
  if (n > max) n = max;
  float nearest = NAN;
  for (int i = 0; i < n; i++) {
    const uint8_t *q = p + 4 + 20 * i;
    float d = sqrtf(rdf(q) * rdf(q) + rdf(q + 4) * rdf(q + 4));
    if (std::isfinite(d) && (!std::isfinite(nearest) || d < nearest)) nearest = d;
  }
  this->last_targets_ms_ = millis();
  this->cleared_ = false;
#ifdef USE_BINARY_SENSOR
  if (this->occupancy_ != nullptr) this->occupancy_->publish_state(n > 0);
#endif
#ifdef USE_SENSOR
  if (this->people_ != nullptr && (!this->people_->has_state() || this->people_->state != n)) this->people_->publish_state(n);
  if (this->nearest_ != nullptr) {
    float r = std::isfinite(nearest) ? roundf(nearest * 100.0f) / 100.0f : NAN;
    if (!this->nearest_->has_state() || this->nearest_->state != r) this->nearest_->publish_state(r);
  }
#endif
}

void KymopsisRadar::on_frame_(uint16_t type, const uint8_t *p, uint16_t len) {
  switch (type) {
    case 0x0A04:
      this->on_targets_(p, len);
      break;
#ifdef USE_BINARY_SENSOR
    case 0x0A0A:
      if (len >= 16)
        for (int i = 0; i < 4; i++)
          if (this->area_[i] != nullptr) this->area_[i]->publish_state(rdi(p + 4 * i) != 0);
      break;
    case 0x0E02:
      if (len >= 1 && this->fall_ != nullptr) this->fall_->publish_state(p[0] != 0);
      break;
    case 0x0F09:
      if (len >= 1 && this->occupancy_ != nullptr) this->occupancy_->publish_state(p[0] != 0);
      break;
#endif
#ifdef USE_SENSOR
    case 0x0A14:
      if (len >= 4 && this->breathing_ != nullptr && rdf(p) > 0) this->breathing_->publish_state(rdf(p));
      break;
    case 0x0A15:
      if (len >= 4 && this->heart_ != nullptr && rdf(p) > 0) this->heart_->publish_state(rdf(p));
      break;
#endif
    default:
      break;
  }
}

}  // namespace kymopsis_radar
}  // namespace esphome
