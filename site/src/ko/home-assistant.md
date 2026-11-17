---
title: Home Assistant
nav: Home Assistant
group: Smart home
---
# Home Assistant

> 다음 단계로 제공 예정입니다. 이 페이지는 연동이 어떻게 동작할지 설명하며, 현재 개발 중입니다.

Radar Base는 MQTT 디스커버리를 통해 Home Assistant에 자동으로 나타납니다. MQTT 브로커는 직접 사용하는 것을 씁니다.
일반적으로는 Home Assistant의 **Mosquitto broker** 애드온을 씁니다. Kymopsis는 브로커도 클라우드도 운영하지 않습니다.

## 예정된 엔티티

| 엔티티 | 종류 | 출처 |
|---|---|---|
| 재실 | 이진 센서 | 3D 재실 감지 |
| 인원 수 | 센서 | 3D 재실 감지 |
| 구역 1-4 사용 중 | 이진 센서 | 3D 재실 감지 |
| 낙상 감지됨 | 이진 센서 및 이벤트 | 낙상 감지 |
| 호흡수, 심박수 | 센서(진단) | 호흡 및 심박수 |
| 설치 방식(천장 / 벽) | 선택 | 3D 재실 감지 |

## 예정된 설정 방법

1. Home Assistant에서 Mosquitto broker 애드온과 MQTT 통합 구성요소를 설치합니다.
2. Radar Base 앱에서: 기기 설정 → 스마트홈 → 브로커 주소, 사용자, 비밀번호를 입력합니다.
3. Radar Base가 설정 → 기기 아래에 나타납니다.

호흡수와 심박수는 웰니스 용도로만 사용하세요. 의료용이 아닙니다.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
