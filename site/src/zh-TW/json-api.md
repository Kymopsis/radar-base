---
title: JSON 結果與 API
nav: JSON 與 API
group: Develop
---
# JSON 結果與 API

所有用戶端 - 應用程式、Home Assistant、你自己的程式碼 - 讀取的都是同一種結果：每行一個 JSON 物件，
結構版本 0。

## 在哪裡讀取

| 連線方式 | 方法 |
|---|---|
| USB | 在序列主控台傳送 `jsonout on`；JSON 行會夾在記錄行之間輸出 |
| Wi-Fi | WebSocket `ws://kymradar.local/ws`（每則訊息一行或多行） |

## 訊息

| `t` | 欄位 | 說明 |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | 情境啟動時、安裝方式變更時，以及每 5 秒傳送 |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | 單位為公尺。x 向右，y 遠離雷達（裝在天花板時為地面平面），z 向上 |
| `zones` | `zones: [{id, occupied}]` | 區域 1-4；變更時和每秒傳送 |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | 變更時和每秒傳送 |
| `vital` | `ts`, `br`, `hr`（每分鐘）, `dist` (m) | 沒有量測對象時省略速率 |

每則訊息都帶有 `"v": 0`。請忽略其他版本的訊息。

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## 指令（USB 主控台或應用程式）

| 指令 | 功能 |
|---|---|
| `ver` | 向雷達應用查詢版本 |
| `ram <file>` | 從雷達記憶體執行雷達應用 |
| `reset` | 從快閃記憶體重新啟動雷達 |
| `jsonout on/off` | 透過 USB 輸出 JSON 結果 |
| `tf 0201 13000000` / `14000000` | 3D 人員存在：天花板 / 牆面安裝 |
| `cfg [key value]` | 原始資料設定 |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | 家用 Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | 以一行 JSON 回傳裝置資訊（應用程式透過 USB 使用） |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | 透過你自己的 MQTT 代理（broker）連接 Home Assistant |
| `board [profile]` | 顯示或設定板型設定檔 |
| `u0raw on/off` | 原樣轉發雷達應用連接埠的位元組（原始擷取） |
| `batch start <firmware>`, `batch stop` | 批次寫入（開發套件韌體） |

## HTTP API（Wi-Fi）

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`。憑證只會放在 POST 請求本文中傳送。

Python 範例用戶端：在 `pc-gui` 資料夾中執行 `python app_views.py --serial COM5`。

大量採購、預燒錄模組或客製韌體： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
