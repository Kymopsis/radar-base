---
title: FAQ and troubleshooting
nav: FAQ
group: Help
---
# FAQ and troubleshooting

## The app connects but shows no results

- Is the radar carrier powered? Was Radar Base powered first? Open Diagnostics → **Restart the radar**.
- Diagnostics → **Ask the radar its version**: a reply means the result wires (RX0/TX0) are fine.

## Loading a scenario fails

- Diagnostics → **Check the boot link**. No reply usually means a loose jumper wire on RX1, TX1, RST, P12 or P19.
- Re-seat the wires; jumper wires lose contact easily. Keep them under 15 cm.

## The raw-data self-test shows corrupted or missing blocks

- Check the SPI CLK and MOSI wires (MOSI, not MISO) and keep a ground wire next to them.
- Lower the SPI speed: console `cfg spi 12.5`.

## Every block is bad and contains only 00 or FF

The data wire is on the carrier's MISO pin. Move it to **MOSI**.

## The browser does not list the USB port

- Use the port marked **USB** on the controller, not UART.
- Close any serial terminal: only one program can use the port.
- Use Chrome or Edge on a desktop computer.

## http://kymradar.local does not open

Some Android phones do not resolve `.local` names. Use the IP address shown under Device settings → Home Wi-Fi.

## Breathing or heart rate stays empty

Sit or lie still 0.5-1.5 m in front of the radar, chest facing it, and wait about 20 seconds. These readings are for
wellness only, not for medical use.

## A person is shown but the area is not lit (or the other way round)

Areas and tracking are separate radar outputs. A person sitting very still can drop out of tracking while the area
stays occupied. The app shows the highest count of the last moment to keep the number steady.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
