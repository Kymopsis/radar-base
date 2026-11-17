---
title: JSON 结果与 API
nav: JSON 与 API
group: Develop
---
# JSON 结果与 API

所有客户端 - 应用、Home Assistant、你自己的代码 - 读取的都是同一种结果：每行一个 JSON 对象，
模式版本 0。

## 在哪里读取

| 连接方式 | 方法 |
|---|---|
| USB | 在串口控制台发送 `jsonout on`；JSON 行会夹在日志行之间输出 |
| Wi-Fi | WebSocket `ws://kymradar.local/ws`（每条消息一行或多行） |

## 消息

| `t` | 字段 | 说明 |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | 场景启动时、安装方式变化时，以及每 5 秒发送 |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | 单位为米。x 向右，y 远离雷达（吸顶时为地面平面），z 向上 |
| `zones` | `zones: [{id, occupied}]` | 区域 1-4；变化时和每秒发送 |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | 变化时和每秒发送 |
| `vital` | `ts`, `br`, `hr`（每分钟）, `dist` (m) | 没有测量对象时省略速率 |

每条消息都带有 `"v": 0`。请忽略其他版本的消息。

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## 命令（USB 控制台或应用）

| 命令 | 作用 |
|---|---|
| `ver` | 向雷达应用查询版本 |
| `ram <file>` | 从雷达内存运行雷达应用 |
| `reset` | 从闪存重启雷达 |
| `jsonout on/off` | 通过 USB 输出 JSON 结果 |
| `tf 0201 13000000` / `14000000` | 3D 存在：吸顶 / 墙面安装 |
| `cfg [key value]` | 原始数据设置 |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | 家庭 Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | 以一行 JSON 返回设备信息（应用通过 USB 使用） |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | 通过你自己的 MQTT 代理（broker）连接 Home Assistant |
| `board [profile]` | 显示或设置板型配置 |
| `u0raw on/off` | 原样转发雷达应用端口的字节（原始抓取） |
| `batch start <firmware>`, `batch stop` | 批量写入（开发套件固件） |

## HTTP API（Wi-Fi）

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`。凭据只会放在 POST 请求体中发送。

Python 示例客户端：在 `pc-gui` 文件夹中运行 `python app_views.py --serial COM5`。

批量采购、预刷固件模块或定制固件： [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
