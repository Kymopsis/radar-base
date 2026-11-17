#include "KymopsisRadar.h"
#include <string.h>
#include <math.h>

// Frame: 0x01, id (2), length (2), type (2) big-endian, ~XOR of those 7 bytes; payload; ~XOR of the payload.
// Results: 0x0A04 targets (int32 n, n x {float x, y, z; int32 doppler; int32 id}); 0x0A0A areas (4 x uint32);
//          0x0E02 uint8 fall; 0x0F09 uint8 someone present; 0x0A14 float breathing /min; 0x0A15 float heart /min.

static float rdf(const uint8_t *p) { float f; memcpy(&f, p, 4); return f; }
static int32_t rdi(const uint8_t *p) { int32_t v; memcpy(&v, p, 4); return v; }

bool KymopsisRadar::update() {
  decoded_ = false;
  int n = 0;
  while (uart_.available() && n++ < 512) byte_((uint8_t) uart_.read());
  return decoded_;
}

void KymopsisRadar::sendFrame(uint16_t type, const uint8_t *data, uint16_t len) {
  uint8_t h[8] = {0x01, (uint8_t) (tx_id_ >> 8), (uint8_t) tx_id_, (uint8_t) (len >> 8), (uint8_t) len,
                  (uint8_t) (type >> 8), (uint8_t) type, 0};
  uint8_t x = 0;
  for (int i = 0; i < 7; i++) x ^= h[i];
  h[7] = ~x;
  uart_.write(h, 8);
  if (len) {
    x = 0;
    for (uint16_t i = 0; i < len; i++) x ^= data[i];
    uart_.write(data, len);
    uart_.write((uint8_t) ~x);
  }
  tx_id_ = tx_id_ == 0xFFFF ? 0x8001 : tx_id_ + 1;
}

void KymopsisRadar::setMount(bool side) {
  uint32_t q = side ? 0x14 : 0x13;
  sendFrame(0x0201, (const uint8_t *) &q, 4);
}

void KymopsisRadar::byte_(uint8_t c) {
  switch (st_) {
    case 0:
      if (c == 0x01) { head_[0] = c; got_ = 1; st_ = 1; }
      break;
    case 1:
      head_[got_++] = c;
      if (got_ == 8) {
        uint8_t x = 0;
        for (int i = 0; i < 7; i++) x ^= head_[i];
        if ((uint8_t) ~x != head_[7]) { st_ = 0; break; }
        len_ = head_[3] << 8 | head_[4];
        type_ = head_[5] << 8 | head_[6];
        got_ = 0;
        if (len_ == 0) { frame_(type_, nullptr, 0); st_ = 0; }
        else st_ = 2;
      }
      break;
    default:
      if (got_ < len_) {
        if (got_ < sizeof pl_) pl_[got_] = c;
        got_++;
        break;
      }
      st_ = 0;
      if (len_ > sizeof pl_) break;          // long frames (point clouds) are skipped
      {
        uint8_t x = 0;
        for (uint16_t i = 0; i < len_; i++) x ^= pl_[i];
        if ((uint8_t) ~x != c) break;
      }
      frame_(type_, pl_, len_);
      break;
  }
}

void KymopsisRadar::frame_(uint16_t type, const uint8_t *p, uint16_t len) {
  decoded_ = true;
  last_ms_ = millis();
  switch (type) {
    case 0x0A04: {
      if (len < 4) break;
      int32_t n = rdi(p), max = (len - 4) / 20;
      if (n < 0) n = 0;
      if (n > max) n = max;
      if (n > MAX_TARGETS) n = MAX_TARGETS;
      people_ = n;
      nearest_ = NAN;
      for (int i = 0; i < n; i++) {
        const uint8_t *q = p + 4 + 20 * i;
        targets_[i] = {rdf(q), rdf(q + 4), rdf(q + 8), rdi(q + 16)};
        float d = sqrtf(targets_[i].x * targets_[i].x + targets_[i].y * targets_[i].y);
        if (isfinite(d) && (!isfinite(nearest_) || d < nearest_)) nearest_ = d;
      }
      break;
    }
    case 0x0A0A:
      if (len >= 16) {
        areas_ = 0;
        for (int i = 0; i < 4; i++) if (rdi(p + 4 * i)) areas_ |= 1 << i;
      }
      break;
    case 0x0E02: if (len >= 1) fall_ = p[0] ? 1 : 0; break;
    case 0x0F09: if (len >= 1) human_ = p[0] ? 1 : 0; break;
    case 0x0A14: if (len >= 4 && rdf(p) > 0) breathing_ = rdf(p); break;
    case 0x0A15: if (len >= 4 && rdf(p) > 0) heart_ = rdf(p); break;
    default: break;
  }
  if (cb_) cb_(type, p, len);
}
