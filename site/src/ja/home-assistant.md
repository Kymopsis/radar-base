---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> 近日対応予定。このページは連携の仕組みを説明するもので、現在開発中です。

Radar Base は MQTT ディスカバリーで Home Assistant に自動的に表示されます。MQTT ブローカーはご自身のものを使います。
一般的なのは Home Assistant の **Mosquitto broker** アドオンです。Kymopsis はブローカーもクラウドも運用しません。

## 予定しているエンティティ

| エンティティ | 種類 | 元になる機能 |
|---|---|---|
| 在室 | バイナリセンサー | 3D 在室 |
| 人数 | センサー | 3D 在室 |
| エリア 1-4 の使用中 | バイナリセンサー | 3D 在室 |
| 転倒を検知 | バイナリセンサーとイベント | 転倒検知 |
| 呼吸数、心拍数 | センサー（診断） | 呼吸と心拍 |
| 設置方法（天井 / 壁） | セレクト | 3D 在室 |

## 予定しているセットアップ

1. Home Assistant で、Mosquitto broker アドオンと MQTT 連携をインストールします。
2. Radar Base アプリで：デバイス設定 → スマートホーム → ブローカーのアドレス、ユーザー、パスワードを入力します。
3. Radar Base が 設定 → デバイス に表示されます。

呼吸数と心拍数はウェルネス用途のみで、医療用途には使えません。

大量購入、書き込み済みモジュール、カスタムファームウェア： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
