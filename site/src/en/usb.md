---
title: Use it over USB in the browser
nav: USB in the browser
group: Connect
---
# Use it over USB in the browser

The Radar Base app runs in Chrome or Edge on your computer and talks to the board through the USB cable
(Web Serial). Nothing to install, no Wi-Fi involved.

## Steps

1. Connect the controller's **USB** port to the computer.
2. Open **[radar.kymopsis.com/app](app/)** in Chrome or Edge.
3. Press **Connect USB** in the top bar and choose "USB JTAG/serial debug unit".
4. The status turns green. Pick a task on the home page.

Your browser asks once per site for permission to use the port; nothing else on your computer is shared.

## What works over USB

| Feature | Over USB |
|---|---|
| Run a scenario, live views, scenario settings | Yes |
| Raw data settings and the data-link self-test | Yes |
| Write firmware to the radar (dev kit) | Yes |
| Set up home Wi-Fi | Yes (network names and passwords without spaces) |
| Upload or delete firmware files | No - use Wi-Fi for this |

## Only one program at a time

A USB port can be open in one program only. Close any serial terminal before connecting the browser, and the
other way round.

## Browsers

Web Serial is available in Chrome, Edge and other Chromium-based browsers on desktop. Safari and Firefox do not
support it; use [Home Wi-Fi](wifi) on those, or on phones.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
