---
title: Put Radar Base on your home Wi-Fi
nav: Home Wi-Fi
group: Connect
---
# Put Radar Base on your home Wi-Fi

Once Radar Base is on your home network, any phone, tablet or computer at home can open it at
**http://kymradar.local**. Your devices stay on your normal Wi-Fi.

## Set it up

Choose one way:

- **From the app (USB or hotspot):** Device settings → Home Wi-Fi → pick your network → enter the password → Connect.
- **From a serial terminal:** `wifi <network-name> <password>` (no spaces in either).

Radar Base needs a 2.4 GHz network. Most home routers offer 2.4 GHz and 5 GHz under the same name, which is fine.

## Open it

- On computers, iPhones, iPads and Macs: **http://kymradar.local**
- On some Android phones `.local` names do not resolve. Use the IP address shown under Device settings → Home Wi-Fi
  (for example http://192.168.1.57), or look the device "kymradar" up in your router's list.

## What happens to the hotspot

The Radar Base hotspot switches itself off once your home Wi-Fi works and no phone is connected to it. It comes
back on its own if the home network is gone for 30 seconds - see [Hotspot fallback](hotspot).

## Change or remove the network

Device settings → Home Wi-Fi → **Forget network**, or on the console `wifi forget`. The hotspot turns on again.

## Privacy

Radar Base only talks to devices on your own network. It has no cloud connection and sends no data anywhere.

Volume, pre-flashed modules or custom firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
