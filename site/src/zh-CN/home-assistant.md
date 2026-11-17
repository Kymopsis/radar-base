---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> 即将推出。本页说明集成的工作方式，目前正在开发中。

Radar Base 将通过 MQTT 自动发现出现在 Home Assistant 中。你使用自己的 MQTT 代理；常见选择是 Home Assistant 里的
**Mosquitto broker** 加载项。Kymopsis 不运行任何代理，也没有云端。

## 计划中的实体

| 实体 | 类型 | 来源 |
|---|---|---|
| 有人状态 | 二元传感器 | 3D 存在 |
| 人数 | 传感器 | 3D 存在 |
| 区域 1-4 有人 | 二元传感器 | 3D 存在 |
| 检测到跌倒 | 二元传感器和事件 | 跌倒检测 |
| 呼吸率、心率 | 传感器（诊断） | 呼吸与心率 |
| 安装方式（吸顶 / 墙面） | 选择 | 3D 存在 |

## 计划中的设置步骤

1. 在 Home Assistant 中安装 Mosquitto broker 加载项和 MQTT 集成。
2. 在 Radar Base 应用中：设备设置 → 智能家居 → 填入代理地址、用户名和密码。
3. Radar Base 会出现在“设置 → 设备”中。

呼吸和心率仅供健康养生用途，不可用于医疗。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
