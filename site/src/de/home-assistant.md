---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base erscheint per MQTT-Discovery automatisch in Home Assistant. Sie nutzen Ihren eigenen MQTT-Broker, meist
das Add-on **Mosquitto broker** in Home Assistant. Kymopsis betreibt weder einen Broker noch eine Cloud; nichts verlässt Ihr Netzwerk.

## Was Sie brauchen

- Home Assistant mit dem Add-on **Mosquitto broker** und der **MQTT**-Integration oder ein anderer MQTT-Broker, den Sie betreiben.
  Notieren Sie Adresse, Port (meist 1883), Benutzer und Passwort des Brokers.
- Ein **2.4 GHz**-Wi-Fi-Netz oder ein Dualband-Netz, in dem 2.4 GHz und 5 GHz denselben Namen haben. Der ESP32-S3 hat kein
  5-GHz-Funkmodul.
- Radar-Base-Firmware 0.9.1 oder neuer ([im Browser installieren](app)).

## Einrichtung

1. Verbinden Sie sich mit Ihrem Wi-Fi: Radar-Base-App öffnen (per USB oder über den Hotspot des Boards), Geräteeinstellungen, Wi-Fi.
2. In den Geräteeinstellungen unter Smart Home: Broker-Adresse, Port, Benutzer und Passwort eintragen und dann MQTT einschalten.
3. Öffnen Sie in Home Assistant Einstellungen → Geräte & Dienste → MQTT. **Kymopsis Radar Base** ist dort mit seinen Entitäten.
   Sonst ist nichts zu konfigurieren.

## Entitäten

| Entität | Typ | Einheit / Werte | Aus der Radar-App |
|---|---|---|---|
| Belegung | Binärsensor | an / aus | 3D-Anwesenheit |
| Personenzahl | Sensor | Anzahl | 3D-Anwesenheit |
| Nächste Person | Sensor | m | 3D-Anwesenheit |
| Bereich 1, Bereich 2, Bereich 3, Bereich 4 | Binärsensor | an / aus | 3D-Anwesenheit |
| Sturz erkannt | Binärsensor | an / aus | Sturzerkennung |
| Atemfrequenz | Sensor | /min | Atmung und Herzfrequenz |
| Herzfrequenz | Sensor | /min | Atmung und Herzfrequenz |
| Radar-App | Sensor | Name der laufenden App | alle |
| Montage | Auswahl | Decke / Wand | 3D-Anwesenheit |
| Radar neu starten | Taste | | alle |

Werte, die die laufende Radar-App nicht meldet, bleiben leer oder unbekannt. Atem- und Herzfrequenz dienen nur dem Wohlbefinden und nicht der medizinischen Nutzung.

## Online und offline

Das Board veröffentlicht ein Verfügbarkeits-Topic. Schalten Sie MQTT am Board aus, wird das Gerät sofort nicht verfügbar.
Verliert das Board Strom oder Wi-Fi, markiert der Broker es nach etwa 45 Sekunden als nicht verfügbar; es kehrt von selbst zurück, sobald das Board wieder verbunden ist.

## Fehlerbehebung

| Problem | Was zu tun ist |
|---|---|
| Das Gerät erscheint nicht | Prüfen Sie die Smart-Home-Seite des Boards: Sie muss „verbunden“ anzeigen. Prüfen Sie Benutzer und Passwort im Broker. |
| Das Board verbindet sich nicht mit dem Wi-Fi | Nutzen Sie ein 2.4-GHz-Netz oder ein Dualband-Netz mit gemeinsamem Namen; reine 5-GHz-Netze funktionieren nicht. |
| `kymradar.local` lässt sich nicht öffnen | Ein VPN oder Proxy auf diesem Computer kann `.local`-Namen an sich reißen; schalten Sie ihn aus, lassen Sie `.local` direkt laufen oder öffnen Sie die IP-Adresse des Boards. |
| Entfernungen werden in Fuß angezeigt | Home Assistant nutzt Ihr Einheitensystem: Einstellungen → System → Allgemein → Einheitensystem: Metrisch. |

Stückzahlen, vorgeflashte Module oder eigene Firmware: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
