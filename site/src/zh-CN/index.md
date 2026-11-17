---
title: Kymopsis Radar Base
nav: 概览
---
# Radar Base

Radar Base 把 60 GHz 毫米波雷达模块连接到你的电脑、手机和智能家居。
雷达工作由雷达自己的芯片完成；Radar Base 把雷达应用加载进去，把结果转成一种简单的 JSON 格式，并通过 USB 和 Wi-Fi 提供出来。

[[cards]]
- **运行现成的雷达应用** | 3D 人员位置、跌倒检测、呼吸和心率。无需编程。 | getting-started
- **通过 USB 在浏览器中使用** | 插上线，在 Chrome 或 Edge 中打开本站即可。无需 Wi-Fi。 | usb
- **接入家庭 Wi-Fi** | 在家里用任何手机或电脑访问 http://kymradar.local。 | wifi
- **开发自己的集成** | 在任何程序中通过 USB 或 WebSocket 以 JSON 读取结果。 | json-api
[[/cards]]

## 已测试的硬件

| 硬件 | 状态 |
|---|---|
| Kymopsis Radar Base 开发套件 | 已测试。开箱即用；已含板型配置 |
| Kymopsis DIY 方案：ESP32-S3 开发板与载板上的 60 GHz 雷达模块，按[入门](getting-started)接线 | 已测试。开箱即用；已含板型配置 |
| 其他开发板和雷达模块 | 未测试。请自行核对引脚分配和板型配置；应用随后会显示“未知板型，角度未校准” |

## 开放版与开发套件版

| 功能 | 开放固件 | 开发套件固件 |
|---|---|---|
| 从雷达内存运行雷达应用，实时视图 | 是 | 是 |
| 通过 USB 和 Wi-Fi 输出 JSON，11 种语言的网页应用 | 是 | 是 |
| 家庭 Wi-Fi、热点回退、Home Assistant（MQTT） | 是 | 是 |
| 原始数据流、设置与自检 | 是 | 是 |
| 把应用写入雷达自身的闪存，并带备份 | - | 是 |
| 批量写入：PASS / FAIL、计数器、CSV 日志 | - | 是 |
| 出厂设置板型配置 | - | 是 |

## 整体结构

[[diagram]]

| 部分 | 作用 |
|---|---|
| 雷达模块 | 运行雷达应用：每个应用在雷达上完成整条信号处理链并上报结果 |
| Radar Base (ESP32-S3) | 加载雷达应用，把结果转成 JSON，推送原始数据流，提供网页应用，接入你的 Wi-Fi |
| 网页应用 | 选择任务、查看实时结果、修改设置。Wi-Fi 和 USB 下是同一个应用，支持 11 种语言 |
| 你自己的代码 | 通过 USB 或 WebSocket 读取 JSON 结果 |
| 智能家居 | 通过 MQTT 接入 Home Assistant（自动发现），ESPHome 组件 |

## 连接方式

| 方式 | 适用场景 | 指南 |
|---|---|---|
| USB | 初次体验、开发。随处可用，无需网络 | [浏览器中使用 USB](usb) |
| 家庭 Wi-Fi | 在家里用手机和电脑日常使用 | [家庭 Wi-Fi](wifi) |
| 热点 | 首次设置，或连不上家庭网络时 | [热点备用](hotspot) |
| Home Assistant | 自动化：灯光、提醒、仪表盘 | [Home Assistant](home-assistant) |

Radar Base 永远不需要云端账号。你的雷达数据只留在自己的设备上。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
