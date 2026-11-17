---
title: JSON 결과와 API
nav: JSON과 API
group: Develop
---
# JSON 결과와 API

앱, Home Assistant, 직접 만든 코드 등 모든 클라이언트가 같은 결과를 읽습니다. 한 줄에 JSON 객체 하나이며,
스키마 버전은 0입니다.

## 어디서 읽나

| 연결 | 방법 |
|---|---|
| USB | 시리얼 콘솔에 `jsonout on`을 보내면 JSON 줄이 로그 줄 사이에 들어옵니다 |
| Wi-Fi | WebSocket `ws://kymradar.local/ws`(메시지 하나에 한 줄 이상) |

## 메시지

| `t` | 필드 | 비고 |
|---|---|---|
| `cap` | `app` (`hd3d`, `fall`, `vital`), `mounts`, `mount` | 시나리오가 시작될 때, 설치 방식이 바뀔 때, 그리고 5초마다 전송 |
| `targets` | `ts` (ms), `targets: [{id, x, y, z}]` | 단위는 미터. x는 오른쪽, y는 레이더에서 멀어지는 방향(천장 설치 시 바닥 평면), z는 위쪽 |
| `zones` | `zones: [{id, occupied}]` | 구역 1-4. 변경 시 및 1초마다 전송 |
| `fall` | `state` (`none`, `normal`, `fall`), `count` | 변경 시 및 1초마다 전송 |
| `vital` | `ts`, `br`, `hr`(분당), `dist` (m) | 측정 대상이 없는 동안은 수치를 생략 |

모든 메시지에는 `"v": 0`이 들어 있습니다. 다른 버전의 메시지는 무시하세요.

```
{"v":0,"t":"cap","app":"hd3d","mounts":["ceiling","side"],"mount":"side","fov_deg":55}
{"v":0,"t":"targets","ts":42097,"targets":[{"id":1,"x":0.268,"y":1.42,"z":0.05}]}
```

## 명령(USB 콘솔 또는 앱)

| 명령 | 하는 일 |
|---|---|
| `ver` | 레이더 앱에 버전을 묻습니다 |
| `ram <file>` | 레이더 메모리에서 레이더 앱을 실행합니다 |
| `reset` | 플래시에서 레이더를 다시 시작합니다 |
| `jsonout on/off` | USB로 JSON 결과 출력 |
| `tf 0201 13000000` / `14000000` | 3D 재실 감지: 천장 / 벽 설치 |
| `cfg [key value]` | 원시 데이터 설정 |
| `wifi [ssid password]`, `wifi forget`, `wifi scan` | 집 Wi-Fi |
| `api files/cfg/wifi/scan/mqtt/batch` | 기기 정보를 JSON 한 줄로 반환(앱이 USB에서 사용) |
| `mqtt <host> [port] [user] [pass]`, `mqtt off` | 직접 운영하는 MQTT 브로커를 통한 Home Assistant |
| `board [profile]` | 보드 프로필 표시 또는 설정 |
| `u0raw on/off` | 레이더 앱 포트를 바이트 그대로 전달(원시 캡처) |
| `batch start <firmware>`, `batch stop` | 일괄 기록(개발 키트 펌웨어) |

## HTTP API (Wi-Fi)

`GET /api/files`, `GET/POST /api/cfg`, `GET/POST /api/wifi`, `GET /api/wifi/scan`, `POST /api/run?op=...`,
`POST /api/tf?type=&data=`, `GET /api/log?since=`. 인증 정보는 항상 POST 본문으로만 전송됩니다.

Python 클라이언트 예시: `pc-gui` 폴더에서 `python app_views.py --serial COM5`를 실행하세요.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
