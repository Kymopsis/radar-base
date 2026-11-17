---
title: 把 Radar Base 接上家用 Wi-Fi
nav: 家用 Wi-Fi
group: Connect
---
# 把 Radar Base 接上家用 Wi-Fi

Radar Base 連上家用網路後，家裡任何手機、平板或電腦都可以透過 **http://kymradar.local** 開啟它。
你的裝置繼續使用平常的 Wi-Fi。

## 設定方法

任選一種方式：

- **在應用程式中（USB 或熱點）：** 裝置設定 → 家用 Wi-Fi → 選擇你的網路 → 輸入密碼 → 連線。
- **在序列終端機中：** `wifi <network-name> <password>`（兩者都不可含空格）。

Radar Base 需要 2.4 GHz 網路。大多數家用路由器會在同一個名稱下同時提供 2.4 GHz 和 5 GHz，這樣沒問題。

## 開啟方式

- 電腦、iPhone、iPad 和 Mac：**http://kymradar.local**
- 部分 Android 手機無法解析 `.local` 名稱。請使用「裝置設定 → 家用 Wi-Fi」中顯示的 IP 位址
  （例如 http://192.168.1.57），或在路由器的裝置清單中尋找名為 “kymradar” 的裝置。

## 熱點會怎樣

家用 Wi-Fi 可用、且沒有手機連在熱點上時，Radar Base 的熱點會自動關閉。如果家用網路消失 30 秒，熱點會自行重新開啟，
詳見[熱點備援](hotspot)。

## 變更或移除網路

裝置設定 → 家用 Wi-Fi → **忘記網路**，或在主控台輸入 `wifi forget`。熱點會重新開啟。

## 隱私

Radar Base 只與你自己網路裡的裝置通訊。它沒有雲端連線，也不會把任何資料傳送到別處。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
