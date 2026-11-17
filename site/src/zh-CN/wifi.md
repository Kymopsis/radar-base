---
title: 把 Radar Base 接入家庭 Wi-Fi
nav: 家庭 Wi-Fi
group: Connect
---
# 把 Radar Base 接入家庭 Wi-Fi

Radar Base 接入家庭网络后，家里任何手机、平板或电脑都可以通过 **http://kymradar.local** 打开它。
你的设备继续使用平常的 Wi-Fi。

## 设置方法

任选一种方式：

- **在应用中（USB 或热点）：** 设备设置 → 家庭 Wi-Fi → 选择你的网络 → 输入密码 → 连接。
- **在串口终端中：** `wifi <network-name> <password>`（两者都不能含空格）。

Radar Base 需要 2.4 GHz 网络。大多数家用路由器会在同一个名称下同时提供 2.4 GHz 和 5 GHz，这样没问题。

## 打开方式

- 电脑、iPhone、iPad 和 Mac：**http://kymradar.local**
- 部分 Android 手机无法解析 `.local` 名称。请使用“设备设置 → 家庭 Wi-Fi”下显示的 IP 地址
  （例如 http://192.168.1.57），或在路由器的设备列表里查找名为 “kymradar” 的设备。

## 热点会怎样

家庭 Wi-Fi 可用、且没有手机连在热点上时，Radar Base 的热点会自动关闭。如果家庭网络消失 30 秒，热点会自己重新开启，
详见[热点备用](hotspot)。

## 更改或移除网络

设备设置 → 家庭 Wi-Fi → **忘记网络**，或在控制台输入 `wifi forget`。热点会重新开启。

## 隐私

Radar Base 只与你自己网络里的设备通信。它没有云端连接，也不会把任何数据发送到别处。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
