---
title: Kymopsis Radar Base
nav: 개요
---
# Radar Base

Radar Base는 60 GHz 밀리미터파 레이더 모듈을 컴퓨터, 휴대폰, 스마트홈에 연결합니다.
레이더 연산은 레이더 자체 칩이 처리합니다. Radar Base는 레이더 앱을 불러오고, 그 결과를 하나의 간단한 JSON 형식으로 바꿔
USB와 Wi-Fi로 제공합니다.

[[cards]]
- **완성된 레이더 앱 실행** | 3D 사람 위치, 낙상 감지, 호흡수와 심박수. 코딩이 필요 없습니다. | getting-started
- **USB로 브라우저에서 사용** | 연결하고 Chrome 또는 Edge에서 이 사이트를 열면 끝. Wi-Fi가 필요 없습니다. | usb
- **집 Wi-Fi에 연결** | 집 안의 어떤 휴대폰이나 컴퓨터에서도 http://kymradar.local로 접속하세요. | wifi
- **나만의 연동 개발** | 어떤 프로그램에서든 USB나 WebSocket으로 결과를 JSON으로 읽으세요. | json-api
[[/cards]]

## 테스트된 하드웨어

| 하드웨어 | 상태 |
|---|---|
| Kymopsis Radar Base 개발 키트 | 테스트 완료. 바로 사용 가능하며 보드 프로필이 포함되어 있습니다 |
| Kymopsis DIY 구성: ESP32-S3 DevKit과 캐리어 보드의 60 GHz 레이더 모듈, [시작하기](getting-started)대로 배선 | 테스트 완료. 바로 사용 가능하며 보드 프로필이 포함되어 있습니다 |
| 기타 보드와 레이더 모듈 | 테스트하지 않음. 핀 배정과 보드 프로필을 직접 확인하세요. 이 경우 앱에 "알 수 없는 보드, 각도 미보정"이 표시됩니다 |

## 오픈 버전과 개발 키트 버전

| 기능 | 오픈 펌웨어 | 개발 키트 펌웨어 |
|---|---|---|
| 레이더 메모리에서 레이더 앱 실행, 실시간 보기 | 예 | 예 |
| USB와 Wi-Fi를 통한 JSON, 11개 언어의 웹 앱 | 예 | 예 |
| 가정용 Wi-Fi, 핫스팟 대체, Home Assistant(MQTT) | 예 | 예 |
| 원시 데이터 스트림, 설정, 자가 테스트 | 예 | 예 |
| 앱을 레이더 자체 플래시에 기록(백업 포함) | - | 예 |
| 일괄 기록: PASS / FAIL, 카운터, CSV 로그 | - | 예 |
| 공장에서 보드 프로필 설정 | - | 예 |

## 전체 구성

[[diagram]]

| 구성 요소 | 하는 일 |
|---|---|
| 레이더 모듈 | 레이더 앱을 실행합니다. 각 앱이 신호 처리 전체를 레이더 안에서 수행하고 결과를 보고합니다 |
| Radar Base (ESP32-S3) | 레이더 앱을 불러오고, 결과를 JSON으로 변환하고, 원시 데이터를 스트리밍하고, 웹 앱을 제공하고, Wi-Fi에 연결합니다 |
| 웹 앱 | 작업을 고르고, 실시간 결과를 보고, 설정을 바꿉니다. Wi-Fi와 USB에서 같은 앱이며 11개 언어를 지원합니다 |
| 직접 만든 코드 | USB나 WebSocket으로 JSON 결과를 읽습니다 |
| 스마트홈 | MQTT로 Home Assistant(자동 검색), ESPHome 컴포넌트 |

## 연결 방법

| 방식 | 사용 시점 | 가이드 |
|---|---|---|
| USB | 첫 사용, 개발. 어디서나 되고 네트워크가 필요 없음 | [브라우저에서 USB 사용](usb) |
| 집 Wi-Fi | 집에서 휴대폰과 컴퓨터로 일상적으로 사용 | [집 Wi-Fi](wifi) |
| 핫스팟 | 처음 설정할 때, 또는 집 네트워크에 닿지 않을 때 | [핫스팟 대체 연결](hotspot) |
| Home Assistant | 자동화: 조명, 알림, 대시보드 | [Home Assistant](home-assistant) |

Radar Base는 클라우드 계정이 필요하지 않습니다. 레이더 데이터는 내 기기에만 남습니다.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
