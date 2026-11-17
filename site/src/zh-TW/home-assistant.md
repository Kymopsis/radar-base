---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> 即將推出。本頁說明整合的運作方式，目前正在開發中。

Radar Base 將透過 MQTT 自動探索出現在 Home Assistant 中。你使用自己的 MQTT 代理伺服器；常見的選擇是 Home Assistant 裡的
**Mosquitto broker** 附加元件。Kymopsis 不運行任何代理伺服器，也沒有雲端。

## 規劃中的實體

| 實體 | 類型 | 來源 |
|---|---|---|
| 有人狀態 | 二元感測器 | 3D 人員存在 |
| 人數 | 感測器 | 3D 人員存在 |
| 區域 1-4 有人 | 二元感測器 | 3D 人員存在 |
| 偵測到跌倒 | 二元感測器和事件 | 跌倒偵測 |
| 呼吸率、心率 | 感測器（診斷） | 呼吸與心率 |
| 安裝方式（天花板 / 牆面） | 選擇 | 3D 人員存在 |

## 規劃中的設定步驟

1. 在 Home Assistant 中安裝 Mosquitto broker 附加元件和 MQTT 整合。
2. 在 Radar Base 應用程式中：裝置設定 → 智慧家庭 → 填入代理伺服器位址、使用者名稱和密碼。
3. Radar Base 會出現在「設定 → 裝置」中。

呼吸和心率僅供健康保健用途，非醫療用途。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
