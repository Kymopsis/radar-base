---
title: Dev-kit radar flashing module
nav: Dev-kit flashing
group: Develop
---
# Dev-kit radar flashing module

Radar Base firmware comes in two editions built from the same source:

| Edition | What it can do |
|---|---|
| Open | Everything on this site: runs every radar app from the radar's memory, JSON, web app, Wi-Fi, smart home |
| Dev kit | Open + one extra module that writes the radar's own flash |

## What the module does

Writing an app into the radar's flash makes it start by itself at power-up, without Radar Base loading it first. The
module backs up the radar's current flash before every write; if the backup fails, nothing is written. Backups can be
put back at any time from the app.

## How the dev-kit firmware is delivered

The dev-kit firmware comes pre-loaded on the dev-kit board. Updates are announced to buyers by e-mail; the dev-kit
firmware is not a public download. The open firmware will be published as source code in a public repository; this page will link it when it is out.

## Batch flashing

For many modules in a row: choose the firmware once, then connect one module after the other. Each module is detected,
optionally backed up, written, verified, started from its flash and asked for its version. The page and the board light
show PASS or FAIL, a counter adds up, and every module gets one line in a CSV log you can download.

## What it does not do

- It does not connect to the internet.
- It does not collect, store or send any data.
- Nothing else in the firmware depends on it.

It is closed because the small helper program it loads into the radar for writing is not ours to publish. On dev-kit
units that helper is stored encrypted with a key unique to each unit.

## Without it

Every radar app also runs from the radar's memory with the open firmware (Run a scenario). You only lose the
"starts by itself without Radar Base" convenience. The open firmware, the app and the JSON format are open by design; the app and the JSON format are documented on this site.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
