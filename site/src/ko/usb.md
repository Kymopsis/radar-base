---
title: 브라우저에서 USB로 사용하기
nav: 브라우저에서 USB
group: Connect
---
# 브라우저에서 USB로 사용하기

Radar Base 앱은 컴퓨터의 Chrome 또는 Edge에서 실행되며, USB 케이블(Web Serial)로 보드와 통신합니다.
설치할 것이 없고 Wi-Fi도 필요하지 않습니다.

## 순서

1. 컨트롤러의 **USB** 포트를 컴퓨터에 연결합니다.
2. Chrome 또는 Edge에서 **[radar.kymopsis.com/app](app/)**을 엽니다.
3. 상단 바의 **USB 연결**을 누르고 "USB JTAG/serial debug unit"을 선택합니다.
4. 상태가 초록색으로 바뀝니다. 홈 화면에서 작업을 고르세요.

브라우저는 사이트마다 한 번만 포트 사용 권한을 묻습니다. 컴퓨터의 다른 내용은 공유되지 않습니다.

## USB로 되는 기능

| 기능 | USB에서 |
|---|---|
| 시나리오 실행, 실시간 보기, 시나리오 설정 | 가능 |
| 원시 데이터 설정과 데이터 링크 자가 테스트 | 가능 |
| 레이더에 펌웨어 기록(개발 키트) | 가능 |
| 집 Wi-Fi 설정 | 가능(네트워크 이름과 비밀번호에 공백이 없어야 함) |
| 펌웨어 파일 업로드 또는 삭제 | 불가 - Wi-Fi를 사용하세요 |

## 한 번에 한 프로그램만

USB 포트는 한 프로그램에서만 열 수 있습니다. 브라우저로 연결하기 전에 시리얼 터미널을 닫으세요.
반대의 경우도 마찬가지입니다.

## 브라우저

Web Serial은 데스크톱의 Chrome, Edge 및 기타 Chromium 기반 브라우저에서 사용할 수 있습니다. Safari와 Firefox는
지원하지 않습니다. 해당 브라우저나 휴대폰에서는 [집 Wi-Fi](wifi)를 사용하세요.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
