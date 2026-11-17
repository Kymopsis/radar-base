---
title: 快速上手
nav: 快速上手
group: Start
---
# 快速上手

本指南带你从开箱到看到实时雷达结果，大约 15 分钟。

## 你需要准备

- Radar Base 控制器（ESP32-S3 板）和装在载板上的 60 GHz 雷达模块
- 10 根杜邦线（母对母）- 套件中已附带
- 两根 USB 线：一根接 Radar Base，一根给雷达载板供电
- 一台装有 Chrome 或 Edge 的电脑（Windows、macOS、Linux 或 ChromeOS）

## 1. 准备雷达载板

取下载板串口排针（J9）和 P19 / GND / P12 启动排针上的所有跳线帽。
否则载板自带的 USB 串口芯片会和 Radar Base 抢用同一组导线。

## 2. 接线

先全部断电。**先接 GND**，再接其他线。标签以雷达载板上印的为准。

[[wiring]]

| Radar Base 引脚 | 雷达载板标签 | 用途 |
|---|---|---|
| GND | GND | 公共地（先接） |
| GPIO17 | RX0 | 向雷达应用发送命令 |
| GPIO18 | TX0 | 接收雷达应用的结果 |
| GPIO15 | RX1 | 向雷达加载应用 |
| GPIO16 | TX1 | 加载时的回复 |
| GPIO4 | RST | 雷达复位 |
| GPIO5 | P12 | 启动选择（也用于数据选择） |
| GPIO6 | P19 | 启动选择 |
| GPIO12 | SPI CLK | 原始数据时钟（仅原始数据用） |
| GPIO11 | SPI MOSI | 原始数据（不要用 MISO） |

雷达由它自己的载板供电（最大约 600 mA）。千万不要从控制板的 3V3 引脚给它供电。

## 3. 按正确顺序上电

1. 把控制器上标有 **USB** 的端口接到电脑。（标有 UART 的端口只用于更新控制器自身的固件。）
2. 然后再给雷达载板供电。

Radar Base 一启动就会设置雷达的启动引脚，所以雷达必须后上电。如果顺序反了，打开“诊断”并点击 **重启雷达**。

## 4. 看它运行

1. 在 Chrome 或 Edge 中打开[应用](app/)，点击 **连接 USB**，选择名为 “USB JTAG/serial” 的端口。
2. 选择 **运行场景** → **3D 存在与人员追踪**。
3. 在雷达前走动。你的位置会显示为一个蓝点。

下一步：日常使用请看[家庭 Wi-Fi](wifi)，想做自己的算法请看[原始数据](raw-data)。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
