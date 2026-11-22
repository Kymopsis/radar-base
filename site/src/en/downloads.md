---
title: Downloads
nav: Downloads
group: Develop
---
# Downloads

## Firmware

The easiest way is the **Install firmware** button on the [home page](app) (Chrome or Edge on a desktop, board on
USB). The same files are on GitHub: [Radar Base releases](https://github.com/Kymopsis/radar-base/releases).
Firmware 0.9.2 and later include the radar apps (3D presence; breathing and heart rate).

## Radar Studio for Windows

[Download Radar Studio 0.9.1 for Windows](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
(64-bit, about 60 MB, no installation needed; SHA-256 in the
[release](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)).

| Works with | Features |
|---|---|
| Any Radar Base | Connect over USB, scenario views, common settings, smart home (MQTT) setup |
| Kymopsis Radar Base dev kit | Also raw data, range-Doppler, waveform and filter settings, your own algorithms, vital signs / HRV |

### "Windows protected your PC"

Radar Studio is not code-signed yet, so Windows SmartScreen may stop it the first time. Choose **More info**, then
**Run anyway**. Check the file's SHA-256 against the release page if you want to be sure it is the original.

## Source code

Open firmware, web app, ESPHome component and Arduino library: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
