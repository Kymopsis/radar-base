---
title: Raw data and your own algorithms
nav: Raw data
group: Develop
---
# Raw data and your own algorithms

Radar Base can stream the radar's raw data (every chirp, both receive channels) over USB. The open firmware includes
this stream and its settings; Radar Studio for Windows views and processes it and opens these views with a dev kit ([Downloads](downloads)).

## What the open firmware gives you

| Feature | Where |
|---|---|
| Raw-data settings: chirp, gain, filters, antennas | App → Raw data & radar settings, or console `cfg <key> <value>` |
| Data-link self-test: every block checked | App → Raw data & radar settings → **Run test** |
| Results as JSON for your own code | [JSON and API](json-api) |

## Where to run an algorithm

Prototype on a PC. Then check whether the radar's own FFT and CFAR hardware can do the job; if so it should run on
the radar and only results leave it. Otherwise check the ESP32, and keep it on the PC only if neither fits.

## Check the data link

App → Raw data & radar settings → **Run test**. It runs the raw-data app for a few seconds and checks every block:
corrupted and missing blocks should both be 0.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
