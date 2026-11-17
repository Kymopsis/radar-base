---
title: 在瀏覽器中透過 USB 使用
nav: 瀏覽器中使用 USB
group: Connect
---
# 在瀏覽器中透過 USB 使用

Radar Base 應用程式在你電腦的 Chrome 或 Edge 中執行，透過 USB 線（Web Serial）與板子通訊。
不需要安裝任何東西，也不涉及 Wi-Fi。

## 步驟

1. 把控制器的 **USB** 連接埠接到電腦。
2. 在 Chrome 或 Edge 中開啟 **[radar.kymopsis.com/app](app/)**。
3. 按下頂端列的 **連接 USB**，選擇 “USB JTAG/serial debug unit”。
4. 狀態變綠。在首頁選擇一個任務。

瀏覽器會針對每個網站只詢問一次是否允許使用該連接埠；電腦上的其他內容不會被分享。

## USB 下可用的功能

| 功能 | 透過 USB |
|---|---|
| 執行情境、即時檢視、情境設定 | 支援 |
| 原始資料設定和資料鏈路自我測試 | 支援 |
| 將韌體寫入雷達（開發套件） | 支援 |
| 設定家用 Wi-Fi | 支援（網路名稱和密碼不可含空格） |
| 上傳或刪除韌體檔案 | 不支援 - 請用 Wi-Fi |

## 同一時間只能一個程式使用

一個 USB 連接埠同一時間只能被一個程式開啟。連線瀏覽器之前請關閉任何序列終端機，反過來也一樣。

## 瀏覽器

Web Serial 可用於桌面版 Chrome、Edge 和其他以 Chromium 為基礎的瀏覽器。Safari 和 Firefox 不支援；
在這些瀏覽器或手機上請使用[家用 Wi-Fi](wifi)。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
