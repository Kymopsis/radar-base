---
title: 原始数据与你自己的算法
nav: 原始数据
group: Develop
---
# 原始数据与你自己的算法

Radar Base 可以通过 USB 传输雷达的原始数据（每个 chirp、两个接收通道）。开放版固件包含这个数据流及其设置；Windows 版 Radar Studio 可查看和处理原始数据，并在开发套件上打开这些视图（[下载](downloads)）。

## 开放版固件提供什么

| 功能 | 位置 |
|---|---|
| 原始数据设置：chirp、增益、滤波器、天线 | 应用 → 原始数据与雷达设置，或控制台 `cfg <key> <value>` |
| 数据链路自检：检查每个数据块 | 应用 → 原始数据与雷达设置 → **开始测试** |
| 供你自己的代码使用的 JSON 结果 | [JSON 结果与 API](json-api) |

## 算法该在哪里运行

先在电脑上做原型。然后看雷达自己的 FFT 和 CFAR 硬件能否胜任；如果可以，就应该在雷达上运行，只把结果传出来。
否则再看 ESP32 能否胜任；两者都不行时才留在电脑上。

## 检查数据链路

应用 → 原始数据与雷达设置 → **开始测试**。它会运行原始数据应用几秒钟并检查每个数据块：
损坏块和缺失块都应为 0。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
