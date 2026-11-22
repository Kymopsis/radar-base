---
title: 下载
nav: 下载
group: Develop
---
# 下载

## 固件

最简单的方法是[首页](app)上的 **安装固件** 按钮（桌面版 Chrome 或 Edge，开发板通过 USB 连接）。相同的文件也在 GitHub 上：[Radar Base 发布页](https://github.com/Kymopsis/radar-base/releases)。
固件 0.9.2 及更高版本包含雷达应用（3D 存在；呼吸和心率）。

## Windows 版 Radar Studio

[下载 Windows 版 Radar Studio 0.9.1](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
（64 位，约 60 MB，无需安装；SHA-256 见
[发布页](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)）。

| 适用于 | 功能 |
|---|---|
| 任意 Radar Base | 通过 USB 连接、场景视图、常用设置、智能家居（MQTT）设置 |
| Kymopsis Radar Base 开发套件 | 另外还有原始数据、距离-多普勒、波形和滤波器设置、你自己的算法、生命体征 / HRV |

### Windows 提示“Windows 已保护你的电脑”

Radar Studio 目前还没有代码签名，所以 Windows SmartScreen 第一次可能会拦截它。点击**更多信息**，然后点击**仍要运行**。如果想确认文件是原版，请对照发布页核对文件的 SHA-256。

## 源代码

开放版固件、网页应用、ESPHome 组件和 Arduino 库：[github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base)。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
