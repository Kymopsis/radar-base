# Radar Base firmware (open edition)

ESP-IDF v6.0, ESP32-S3 (16 MB flash, 8 MB PSRAM board).

```
idf.py set-target esp32s3
idf.py -p COMx flash monitor
```

After flashing, the board answers on the USB port marked USB (console) and as Wi-Fi hotspot `KymRadarBase-xxxx`
(password `kymradar`) until you save your home network. Start with the guides at https://radar.kymopsis.com.

| File | Role |
|---|---|
| `main/radar_link.c` | Radar boot pins, both radar UARTs, loading radar apps into the radar's memory, app messages |
| `main/app_json.c` | Radar results -> JSON schema v0 (also feeds MQTT) |
| `main/spi_rx.c`, `adc_format.c` | Raw ADC frames from the radar (SPI slave + DMA), block checks |
| `main/radar_cfg.c` | Raw-data chirp / gain / filter settings |
| `main/web.c`, `app.html`, `i18n/` | Web server, WebSocket and the web app (11 languages) |
| `main/wifi.c` | Home Wi-Fi first, kymradar.local, hotspot fallback with captive portal |
| `main/mqtt_ha.c` | MQTT + Home Assistant discovery (your own broker) |
| `scripts/check_no_loader.py` | Build check: the open image must not contain radar flash-loader content |
| `scripts/check_open_image.py` | Open-build gate: no dev-kit module symbols, strings or partitions in the image |

Console commands and the JSON format: see "JSON and API" on the site or `../pc-gui/SCHEMA.md`.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=github&utm_medium=readme&utm_campaign=radar-base)
