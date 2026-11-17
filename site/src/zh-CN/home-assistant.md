---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

Radar Base 会通过 MQTT 自动发现出现在 Home Assistant 中。你使用自己的 MQTT 代理（broker），通常是 Home Assistant 里的
**Mosquitto broker** 加载项。Kymopsis 不运行任何代理，也没有云端；数据不会离开你的网络。

## 你需要准备

- 带有 **Mosquitto broker** 加载项和 **MQTT** 集成的 Home Assistant，或你自己运行的其他 MQTT 代理。
  记下代理的地址、端口（通常是 1883）、用户名和密码。
- 一个 **2.4 GHz** Wi-Fi 网络，或 2.4 GHz 与 5 GHz 使用同一个名称的双频网络。ESP32-S3 没有
  5 GHz 射频。
- Radar Base 固件 0.9.1 或更高版本（[在浏览器中安装](app)）。

## 设置步骤

1. 连接 Wi-Fi：打开 Radar Base 应用（通过 USB，或板子的热点），设备设置，Wi-Fi。
2. 在设备设置的“智能家居”中：填入代理地址、端口、用户名和密码，然后打开 MQTT。
3. 在 Home Assistant 中打开“设置 → 设备与服务 → MQTT”。**Kymopsis Radar Base** 已在其中，实体齐全。
   无需其他配置。

## 实体

| 实体 | 类型 | 单位 / 取值 | 来自的雷达应用 |
|---|---|---|---|
| 有人状态 | 二元传感器 | 开 / 关 | 3D 存在 |
| 人数 | 传感器 | 数量 | 3D 存在 |
| 最近的人 | 传感器 | m | 3D 存在 |
| 区域 1、区域 2、区域 3、区域 4 | 二元传感器 | 开 / 关 | 3D 存在 |
| 检测到跌倒 | 二元传感器 | 开 / 关 | 跌倒检测 |
| 呼吸率 | 传感器 | /min | 呼吸与心率 |
| 心率 | 传感器 | /min | 呼吸与心率 |
| 雷达应用 | 传感器 | 正在运行的应用名称 | 任意 |
| 安装方式 | 选择 | 吸顶 / 墙面 | 3D 存在 |
| 重启雷达 | 按钮 | | 任意 |

正在运行的雷达应用没有上报的值会保持为空或未知。呼吸和心率仅供健康养生用途，不可用于医疗。

## 在线与离线

板子会发布一个可用性主题。在板子上关闭 MQTT 后，设备会立即变为不可用。
如果板子断电或断开 Wi-Fi，代理会在约 45 秒后将其标记为不可用；板子重新连接后会自动恢复。

## 故障排除

| 问题 | 处理方法 |
|---|---|
| 设备没有出现 | 查看板子的“智能家居”页面：必须显示已连接。检查代理中的用户名和密码。 |
| 板子无法连接 Wi-Fi | 使用 2.4 GHz 网络，或使用同一名称的双频网络；仅 5 GHz 的网络无法使用。 |
| `kymradar.local` 打不开 | 该电脑上的 VPN 或代理可能会接管 `.local` 名称；请关闭它，或让 `.local` 直连，也可以直接打开板子的 IP 地址。 |
| 距离显示为英尺 | Home Assistant 使用你设置的单位制：设置 → 系统 → 通用 → 单位制：公制。 |

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
