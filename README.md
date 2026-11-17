# Kymopsis Radar Base

Open firmware, web app, PC tool and docs for **Radar Base**: an ESP32-S3 board that runs a 60 GHz mmWave radar
module and makes its results available over USB, home Wi-Fi and Home Assistant.

Docs and the web app: **[radar.kymopsis.com](https://radar.kymopsis.com/?utm_source=github&utm_medium=readme&utm_campaign=radar-base)**

## Tested hardware

| Hardware | Status |
|---|---|
| [Kymopsis Radar Base dev kit](https://kymopsis.com/product/60ghz-ceiling-presence-radar-development-kit/?utm_source=github&utm_medium=readme&utm_campaign=radar-base) | Tested. Works out of the box; board profile included |
| Kymopsis DIY build: ESP32-S3 DevKit + 60 GHz radar module on its carrier, wired as in the [getting-started guide](https://radar.kymopsis.com/en/getting-started.html?utm_source=github&utm_medium=readme&utm_campaign=radar-base) | Tested. Works out of the box; board profile included |
| Other boards and radar modules | Untested. Check the pin assignment and the board profile yourself; the app then shows "Unknown board, angles not calibrated" |

## Open and dev-kit editions

| Feature | Open firmware (this repository) | Dev-kit firmware |
|---|---|---|
| Run radar apps from the radar's memory, live views | Yes | Yes |
| JSON over USB and Wi-Fi, web app in 11 languages | Yes | Yes |
| Home Wi-Fi, hotspot fallback, Home Assistant (MQTT) | Yes | Yes |
| Raw-data stream, settings and self-test | Yes | Yes |
| Write an app into the radar's own flash, with backup | - | Yes |
| Batch flashing: PASS / FAIL, counter, CSV log | - | Yes |
| Board profile set at the factory | - | Yes |

The dev-kit firmware adds one small closed module whose only job is writing the radar's own flash (with a backup
first). It does not connect to the internet and collects no data; every radar app also runs from the radar's memory
with this open firmware. See the "Dev-kit flashing" guide.

## What is here

| Folder | What |
|---|---|
| `firmware/` | ESP32-S3 firmware (ESP-IDF v6.0): loads radar apps into the radar, JSON results, web app, Wi-Fi, MQTT / Home Assistant |
| `pc-gui/` | JSON results viewer and example client (Python) |
| `esphome/` | ESPHome external component `kymopsis_radar` and an example YAML |
| `arduino/KymopsisRadar/` | Arduino / PlatformIO library: radar app results on any board wired to the radar's UART |
| `site/` | Guides for radar.kymopsis.com in 11 languages (built by GitHub Actions) |
| `ARCHITECTURE.md` | How it fits together, where algorithms run, developer quick start |

Breathing and heart rate are for wellness use only, not for medical use.

## License

Code: MIT (see `LICENSE`). Guides and documentation in `site/` and the `.md` files: CC BY 4.0 (see `site/LICENSE`).

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=github&utm_medium=readme&utm_campaign=radar-base)
