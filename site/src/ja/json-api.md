---
title: JSON の結果と API
nav: JSON と API
group: Develop
---
# JSON の結果と API

すべてのクライアント - アプリ、Home Assistant、あなた自身のコード - が同じ結果を読みます。1 行に 1 つの JSON オブジェクト、
スキーマバージョン 0 です。

## 読み取り方

| 接続 | 方法 |
|---|---|
| USB | シリアルコンソールで `jsonout on` を送る。JSON の行がログの行の間に流れてくる |
| Wi-Fi | WebSocket `ws://kymradar.local/ws`（1 メッセージに 1 行以上） |

## メッセージ

| `t` | フィールド | 備考 |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | シナリオ開始時、設置方法の変更時、そして 5 秒ごとに送信 |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | 単位はメートル。x は右、y はレーダーから遠ざかる方向（天井設置では床面）、z は上 |
| `zones` | `zones: [{id, occupied}]` | エリア 1-4。変化時と 1 秒ごとに送信 |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | 変化時と 1 秒ごとに送信 |
| `vital` | `ts`, `br`, `hr`（毎分）, `dist` (m) | 誰も測定していない間は数値を省略 |

すべてのメッセージに `"v": 0` が付きます。別のバージョンのメッセージは無視してください。

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## コマンド（USB コンソールまたはアプリ）

| コマンド | 内容 |
|---|---|
| `ver` | レーダーアプリのバージョンを問い合わせる |
| `ram <file>` | レーダーのメモリからレーダーアプリを実行する |
| `reset` | フラッシュからレーダーを再起動する |
| `jsonout on/off` | USB で JSON の結果を出力する |
| `tf 0201 13000000` / `14000000` | 3D 在室：天井 / 壁設置 |
| `cfg [key value]` | 生データの設定 |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | 自宅の Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | デバイス情報を 1 行の JSON で返す（アプリが USB 経由で使用） |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | ご自身の MQTT ブローカー経由の Home Assistant |
| `board [profile]` | ボードプロファイルの表示または設定 |
| `u0raw on/off` | レーダーアプリのポートをバイト単位でそのまま転送（生データ取得） |
| `batch start <firmware>`, `batch stop` | 一括書き込み（開発キット用ファームウェア） |

## HTTP API（Wi-Fi）

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`。認証情報は POST の本文でのみ送られます。

Python のクライアント例：`pc-gui` フォルダーで `python app_views.py --serial COM5` を実行します。

大量購入、書き込み済みモジュール、カスタムファームウェア： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
