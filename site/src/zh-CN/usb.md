---
title: 在浏览器中通过 USB 使用
nav: 浏览器中使用 USB
group: Connect
---
# 在浏览器中通过 USB 使用

Radar Base 应用在你电脑的 Chrome 或 Edge 中运行，通过 USB 线（Web Serial）与板子通信。
无需安装任何东西，也不涉及 Wi-Fi。

## 步骤

1. 把控制器的 **USB** 端口接到电脑。
2. 在 Chrome 或 Edge 中打开 **[radar.kymopsis.com/app](app/)**。
3. 点击顶栏的 **连接 USB**，选择 “USB JTAG/serial debug unit”。
4. 状态变绿。在首页选择一个任务。

浏览器会针对每个网站只询问一次是否允许使用该端口；电脑上的其他内容不会被共享。

## USB 下可用的功能

| 功能 | 通过 USB |
|---|---|
| 运行场景、实时视图、场景设置 | 支持 |
| 原始数据设置和数据链路自检 | 支持 |
| 向雷达写入固件（开发套件） | 支持 |
| 设置家庭 Wi-Fi | 支持（网络名称和密码不能含空格） |
| 上传或删除固件文件 | 不支持 - 请用 Wi-Fi |

## 同一时间只能一个程序使用

一个 USB 端口同一时间只能被一个程序打开。连接浏览器之前请关闭任何串口终端，反过来也一样。

## 浏览器

Web Serial 可用于桌面版 Chrome、Edge 和其他基于 Chromium 的浏览器。Safari 和 Firefox 不支持；
在这些浏览器或手机上请使用[家庭 Wi-Fi](wifi)。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
