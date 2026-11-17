---
title: Radar Base を自宅の Wi-Fi につなぐ
nav: 自宅の Wi-Fi
group: Connect
---
# Radar Base を自宅の Wi-Fi につなぐ

Radar Base が自宅のネットワークに入れば、家にあるスマホ、タブレット、パソコンのどれからでも **http://kymradar.local** で開けます。
それぞれのデバイスは、いつもの Wi-Fi につながったままです。

## 設定する

次のどちらかの方法を選びます。

- **アプリから（USB またはホットスポット）：** デバイス設定 → 自宅の Wi-Fi → ネットワークを選ぶ → パスワードを入力 → 接続。
- **シリアルターミナルから：** `wifi <network-name> <password>`（どちらも空白を含めない）。

Radar Base には 2.4 GHz のネットワークが必要です。多くの家庭用ルーターは、同じ名前で 2.4 GHz と 5 GHz を提供しており、それで問題ありません。

## 開く

- パソコン、iPhone、iPad、Mac：**http://kymradar.local**
- 一部の Android スマホでは `.local` の名前が解決できません。デバイス設定 → 自宅の Wi-Fi に表示される IP アドレス
  （例：http://192.168.1.57）を使うか、ルーターの一覧で「kymradar」というデバイスを探してください。

## ホットスポットはどうなる？

自宅の Wi-Fi が使えて、ホットスポットにスマホがつながっていなくなると、Radar Base のホットスポットは自動で停止します。
自宅のネットワークが 30 秒間なくなると、自動的に再びオンになります。[ホットスポットのフォールバック](hotspot)を参照してください。

## ネットワークを変更・削除する

デバイス設定 → 自宅の Wi-Fi → **ネットワークを削除**、またはコンソールで `wifi forget`。ホットスポットが再びオンになります。

## プライバシー

Radar Base は、自分のネットワーク上のデバイスとだけ通信します。クラウドには接続せず、データをどこにも送信しません。

大量購入、書き込み済みモジュール、カスタムファームウェア： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
