---
title: Radar Base를 집 Wi-Fi에 연결하기
nav: 집 Wi-Fi
group: Connect
---
# Radar Base를 집 Wi-Fi에 연결하기

Radar Base가 집 네트워크에 들어오면, 집 안의 휴대폰, 태블릿, 컴퓨터 어디서나 **http://kymradar.local**로 열 수 있습니다.
기기들은 평소 쓰던 Wi-Fi에 그대로 연결되어 있습니다.

## 설정하기

둘 중 하나를 고르세요.

- **앱에서(USB 또는 핫스팟):** 기기 설정 → 집 Wi-Fi → 네트워크 선택 → 비밀번호 입력 → 연결.
- **시리얼 터미널에서:** `wifi <network-name> <password>`(둘 다 공백 없이).

Radar Base에는 2.4 GHz 네트워크가 필요합니다. 대부분의 가정용 공유기는 같은 이름으로 2.4 GHz와 5 GHz를 함께 제공하며, 그래도 괜찮습니다.

## 열기

- 컴퓨터, iPhone, iPad, Mac: **http://kymradar.local**
- 일부 Android 휴대폰은 `.local` 이름을 해석하지 못합니다. 기기 설정 → 집 Wi-Fi에 표시된 IP 주소
  (예: http://192.168.1.57)를 쓰거나, 공유기 목록에서 "kymradar" 기기를 찾아보세요.

## 핫스팟은 어떻게 되나

집 Wi-Fi가 작동하고 핫스팟에 연결된 휴대폰이 없으면 Radar Base 핫스팟은 스스로 꺼집니다.
집 네트워크가 30초 동안 사라지면 저절로 다시 켜집니다. [핫스팟 대체 연결](hotspot)을 참고하세요.

## 네트워크 변경 또는 삭제

기기 설정 → 집 Wi-Fi → **네트워크 삭제**, 또는 콘솔에서 `wifi forget`. 핫스팟이 다시 켜집니다.

## 개인정보 보호

Radar Base는 내 네트워크의 기기와만 통신합니다. 클라우드에 연결하지 않고, 어떤 데이터도 외부로 보내지 않습니다.

대량 구매, 사전 기록된 모듈, 맞춤 펌웨어: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
