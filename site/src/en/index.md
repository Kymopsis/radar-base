---
title: Kymopsis Radar Base
nav: Overview
---
# Radar Base

Radar Base connects a 60 GHz mmWave radar module to your computer, your phone and your smart home.
The radar does the radar work on its own chip; Radar Base loads radar apps into it, turns their results into
one simple JSON format, and serves them over USB and Wi-Fi.

[[cards]]
- **Run a ready-made radar app** | People positions in 3D, fall detection, breathing and heart rate. No coding. | getting-started
- **Use it in the browser over USB** | Plug in, open this site in Chrome or Edge, done. No Wi-Fi needed. | usb
- **Put it on your home Wi-Fi** | Reach it at http://kymradar.local from any phone or computer at home. | wifi
- **Build your own integration** | Read the results as JSON over USB or WebSocket from any program. | json-api
[[/cards]]

## Tested hardware

| Hardware | Status |
|---|---|
| Kymopsis Radar Base dev kit | Tested. Works out of the box; board profile included |
| Kymopsis DIY build: ESP32-S3 DevKit and the 60 GHz radar module on its carrier, wired as in [Getting started](getting-started) | Tested. Works out of the box; board profile included |
| Other boards and radar modules | Untested. Check the pin assignment and the board profile yourself; the app then shows "Unknown board, angles not calibrated" |

## Open and dev-kit editions

| Feature | Open firmware | Dev-kit firmware |
|---|---|---|
| Run radar apps from the radar's memory, live views | Yes | Yes |
| JSON over USB and Wi-Fi, web app in 11 languages | Yes | Yes |
| Home Wi-Fi, hotspot fallback, Home Assistant (MQTT) | Yes | Yes |
| Raw-data stream, settings and self-test | Yes | Yes |
| Write an app into the radar's own flash, with backup | - | Yes |
| Batch flashing: PASS / FAIL, counter, CSV log | - | Yes |
| Board profile set at the factory | - | Yes |

## How it fits together

[[diagram]]

| Part | What it does |
|---|---|
| Radar module | Runs the radar apps: each app does the whole signal chain on the radar and reports results |
| Radar Base (ESP32-S3) | Loads radar apps, translates results to JSON, streams raw data, serves the web app, joins your Wi-Fi |
| Web app | Pick a task, watch live results, change settings. Same app over Wi-Fi and over USB, in 11 languages |
| Your own code | Reads the JSON results over USB or WebSocket |
| Smart home | Home Assistant over MQTT (auto-discovery), ESPHome component |

## Ways to connect

| Mode | When to use it | Guide |
|---|---|---|
| USB | First steps and development. Works anywhere, no network | [USB in the browser](usb) |
| Home Wi-Fi | Everyday use from phones and computers at home | [Home Wi-Fi](wifi) |
| Hotspot | First setup, or when no home network is reachable | [Hotspot fallback](hotspot) |
| Home Assistant | Automations: lights, alerts, dashboards | [Home Assistant](home-assistant) |

Radar Base never needs a cloud account. Your radar data stays on your own devices.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
