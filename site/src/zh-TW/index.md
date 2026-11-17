---
title: Kymopsis Radar Base
nav: 總覽
---
# Radar Base

Radar Base 把 60 GHz 毫米波雷達模組連接到你的電腦、手機和智慧家庭。
雷達的運算由雷達自己的晶片完成；Radar Base 把雷達應用載入進去，把結果轉成一種簡單的 JSON 格式，並透過 USB 和 Wi-Fi 提供出來。

[[cards]]
- **執行現成的雷達應用** | 3D 人員位置、跌倒偵測、呼吸和心率。不需要寫程式。 | getting-started
- **透過 USB 在瀏覽器中使用** | 插上線，在 Chrome 或 Edge 中開啟本站即可。不需要 Wi-Fi。 | usb
- **接上家用 Wi-Fi** | 在家中用任何手機或電腦連到 http://kymradar.local。 | wifi
- **開發自己的整合** | 在任何程式中透過 USB 或 WebSocket 以 JSON 讀取結果。 | json-api
[[/cards]]

## 已測試的硬體

| 硬體 | 狀態 |
|---|---|
| Kymopsis Radar Base 開發套件 | 已測試。開箱即用；已含板型設定檔 |
| Kymopsis DIY 方案：ESP32-S3 開發板與載板上的 60 GHz 雷達模組，依[入門](getting-started)接線 | 已測試。開箱即用；已含板型設定檔 |
| 其他開發板與雷達模組 | 未測試。請自行核對腳位配置與板型設定檔；應用程式隨後會顯示「未知板型，角度未校準」 |

## 開放版與開發套件版

| 功能 | 開放韌體 | 開發套件韌體 |
|---|---|---|
| 從雷達記憶體執行雷達應用，即時檢視 | 是 | 是 |
| 透過 USB 與 Wi-Fi 輸出 JSON，11 種語言的網頁應用 | 是 | 是 |
| 家用 Wi-Fi、熱點備援、Home Assistant（MQTT） | 是 | 是 |
| 原始資料串流、設定與自我測試 | 是 | 是 |
| 把應用寫入雷達本身的快閃記憶體，並附備份 | - | 是 |
| 批次寫入：PASS / FAIL、計數器、CSV 紀錄 | - | 是 |
| 出廠設定板型設定檔 | - | 是 |

## 整體架構

[[diagram]]

| 部分 | 功能 |
|---|---|
| 雷達模組 | 執行雷達應用：每個應用在雷達上完成整條訊號處理鏈並回報結果 |
| Radar Base (ESP32-S3) | 載入雷達應用，把結果轉成 JSON，串流原始資料，提供網頁應用程式，連上你的 Wi-Fi |
| 網頁應用程式 | 選擇任務、查看即時結果、修改設定。Wi-Fi 和 USB 下是同一個應用程式，支援 11 種語言 |
| 你自己的程式碼 | 透過 USB 或 WebSocket 讀取 JSON 結果 |
| 智慧家庭 | 透過 MQTT 連接 Home Assistant（即將推出），ESPHome（之後） |

## 連線方式

| 方式 | 適用情境 | 指南 |
|---|---|---|
| USB | 初次體驗、開發。隨處可用，不需要網路 | [瀏覽器中使用 USB](usb) |
| 家用 Wi-Fi | 在家中用手機和電腦日常使用 | [家用 Wi-Fi](wifi) |
| 熱點 | 首次設定，或連不上家用網路時 | [熱點備援](hotspot) |
| Home Assistant | 自動化：燈光、提醒、儀表板 | [Home Assistant](home-assistant) |

Radar Base 永遠不需要雲端帳號。你的雷達資料只留在自己的裝置上。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
