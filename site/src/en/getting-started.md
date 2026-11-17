---
title: Getting started
nav: Getting started
group: Start
---
# Getting started

This guide takes you from the box to live radar results in about 15 minutes.

## What you need

- Radar Base controller (ESP32-S3 board) and the 60 GHz radar module on its carrier board
- 10 jumper wires (female to female) - included in the kit
- Two USB cables: one for Radar Base, one to power the radar carrier
- A computer with Chrome or Edge (Windows, macOS, Linux or ChromeOS)

## 1. Prepare the radar carrier

Remove all jumper caps from the carrier's serial header (J9) and from the P19 / GND / P12 boot header.
Otherwise the carrier's own USB-serial chip competes with Radar Base on the same wires.

## 2. Wire it

Power everything off first. Connect **GND first**, then the rest. Labels are the ones printed on the radar carrier.

[[wiring]]

| Radar Base pin | Radar carrier label | Purpose |
|---|---|---|
| GND | GND | Common ground (connect first) |
| GPIO17 | RX0 | Commands to the radar app |
| GPIO18 | TX0 | Results from the radar app |
| GPIO15 | RX1 | Loading apps into the radar |
| GPIO16 | TX1 | Replies while loading |
| GPIO4 | RST | Radar reset |
| GPIO5 | P12 | Boot select (also data select) |
| GPIO6 | P19 | Boot select |
| GPIO12 | SPI CLK | Raw data clock (raw-data use only) |
| GPIO11 | SPI MOSI | Raw data (do not use MISO) |

The radar is powered by its own carrier (it draws up to about 600 mA). Never power it from the 3V3 pin of the
controller board.

## 3. Power up in the right order

1. Plug the controller's port marked **USB** into your computer. (The port marked UART is only for updating the
   controller's own firmware.)
2. Then power the radar carrier.

Radar Base sets the radar's boot pins as soon as it starts, so the radar must come up second. If you powered them
the other way round, open Diagnostics and press **Restart the radar**.

## 4. See it work

1. Open [the app](app/) in Chrome or Edge and press **Connect USB**; pick the port named "USB JTAG/serial".
2. Choose **Run a scenario** → **3D presence & tracking**.
3. Walk in front of the radar. Your position appears as a blue dot.

Next: [Home Wi-Fi](wifi) for everyday use, or [Raw data](raw-data) to build your own algorithms.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
