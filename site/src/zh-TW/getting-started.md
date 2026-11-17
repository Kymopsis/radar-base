---
title: 快速上手
nav: 快速上手
group: Start
---
# 快速上手

本指南帶你從開箱到看見即時雷達結果，大約 15 分鐘。

## 你需要準備

- Radar Base 控制器（ESP32-S3 板）和裝在載板上的 60 GHz 雷達模組
- 10 條杜邦線（母對母）- 套件中已附上
- 兩條 USB 線：一條接 Radar Base，一條替雷達載板供電
- 一台裝有 Chrome 或 Edge 的電腦（Windows、macOS、Linux 或 ChromeOS）

## 1. 準備雷達載板

取下載板序列排針（J9）和 P19 / GND / P12 開機排針上的所有跳線帽。
否則載板自帶的 USB 序列晶片會和 Radar Base 搶用同一組導線。

## 2. 接線

先全部斷電。**先接 GND**，再接其他線。標籤以雷達載板上印的為準。

[[wiring]]

| Radar Base 腳位 | 雷達載板標籤 | 用途 |
|---|---|---|
| GND | GND | 共用接地（先接） |
| GPIO17 | RX0 | 對雷達應用傳送指令 |
| GPIO18 | TX0 | 接收雷達應用的結果 |
| GPIO15 | RX1 | 把應用載入雷達 |
| GPIO16 | TX1 | 載入時的回應 |
| GPIO4 | RST | 雷達重置 |
| GPIO5 | P12 | 開機選擇（也用於資料選擇） |
| GPIO6 | P19 | 開機選擇 |
| GPIO12 | SPI CLK | 原始資料時脈（僅原始資料用） |
| GPIO11 | SPI MOSI | 原始資料（不要用 MISO） |

雷達由它自己的載板供電（最大約 600 mA）。千萬不要從控制板的 3V3 腳位替它供電。

## 3. 依正確順序上電

1. 把控制器上標示 **USB** 的連接埠接到電腦。（標示 UART 的連接埠只用於更新控制器自身的韌體。）
2. 然後再替雷達載板供電。

Radar Base 一啟動就會設定雷達的開機腳位，所以雷達必須後上電。如果順序反了，開啟「診斷」並按下 **重新啟動雷達**。

## 4. 看它運作

1. 在 Chrome 或 Edge 中開啟[應用程式](app/)，按下 **連接 USB**，選擇名為 “USB JTAG/serial” 的連接埠。
2. 選擇 **執行情境** → **3D 人員存在與追蹤**。
3. 在雷達前走動。你的位置會顯示為一個藍點。

下一步：日常使用請看[家用 Wi-Fi](wifi)，想做自己的演算法請看[原始資料](raw-data)。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
