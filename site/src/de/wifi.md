---
title: Radar Base ins Heim-Wi-Fi bringen
nav: Heim-Wi-Fi
group: Connect
---
# Radar Base ins Heim-Wi-Fi bringen

Sobald Radar Base in Ihrem Heimnetz ist, kann jedes Handy, Tablet oder jeder Computer zu Hause es unter
**http://kymradar.local** öffnen. Ihre Geräte bleiben in Ihrem normalen Wi-Fi.

## Einrichten

Wählen Sie einen Weg:

- **In der App (USB oder Hotspot):** Geräteeinstellungen → Heim-Wi-Fi → Netzwerk wählen → Passwort eingeben → Verbinden.
- **Im seriellen Terminal:** `wifi <network-name> <password>` (in beiden keine Leerzeichen).

Radar Base braucht ein 2,4-GHz-Netz. Die meisten Heim-Router bieten 2,4 GHz und 5 GHz unter demselben Namen an, das ist in Ordnung.

## Öffnen

- Auf Computern, iPhones, iPads und Macs: **http://kymradar.local**
- Auf manchen Android-Handys lassen sich `.local`-Namen nicht auflösen. Verwenden Sie die IP-Adresse unter
  Geräteeinstellungen → Heim-Wi-Fi (zum Beispiel http://192.168.1.57) oder suchen Sie das Gerät „kymradar“ in der Liste
  Ihres Routers.

## Was mit dem Hotspot passiert

Der Hotspot von Radar Base schaltet sich von selbst aus, sobald Ihr Heim-Wi-Fi funktioniert und kein Handy mit ihm
verbunden ist. Er geht von selbst wieder an, wenn das Heimnetz 30 Sekunden lang weg ist - siehe [Hotspot als Rückfall](hotspot).

## Netzwerk ändern oder entfernen

Geräteeinstellungen → Heim-Wi-Fi → **Netzwerk vergessen**, oder auf der Konsole `wifi forget`. Der Hotspot geht wieder an.

## Datenschutz

Radar Base spricht nur mit Geräten in Ihrem eigenen Netzwerk. Es hat keine Cloud-Verbindung und sendet nirgendwohin Daten.

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
