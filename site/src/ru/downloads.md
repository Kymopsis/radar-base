---
title: Загрузки
nav: Загрузки
group: Develop
---
# Загрузки

## Прошивка

Проще всего нажать кнопку **Установить прошивку** на [главной странице](app) (Chrome или Edge на компьютере, плата по USB). Те же файлы есть на GitHub: [релизы Radar Base](https://github.com/Kymopsis/radar-base/releases).
Прошивка 0.9.2 и новее включает приложения радара (3D-присутствие; дыхание и пульс).

## Radar Studio для Windows

[Скачать Radar Studio 0.9.1 для Windows](https://github.com/Kymopsis/radar-base/releases/download/v0.9.1/KymopsisRadarStudio-0.9.1.exe)
(64 бита, около 60 МБ, установка не нужна; SHA-256 указан в
[релизе](https://github.com/Kymopsis/radar-base/releases/tag/v0.9.1)).

| Работает с | Возможности |
|---|---|
| Любой Radar Base | Подключение по USB, сценарные виды, основные настройки, настройка умного дома (MQTT) |
| Комплект разработчика Kymopsis Radar Base | Дополнительно сырые данные, дальность-Доплер, настройки формы сигнала и фильтров, ваши алгоритмы, жизненные показатели / HRV |

### «Windows защитила ваш компьютер»

Radar Studio пока не подписан цифровой подписью, поэтому Windows SmartScreen может остановить его при первом запуске. Выберите **Подробнее**, затем **Выполнить в любом случае**. Сверьте SHA-256 файла со страницей релиза, если хотите убедиться, что это оригинал.

## Исходный код

Открытая прошивка, веб-приложение, компонент ESPHome и библиотека Arduino: [github.com/Kymopsis/radar-base](https://github.com/Kymopsis/radar-base).

Партии, предварительно прошитые модули или индивидуальная прошивка: [Kymopsis B2B](https://kymopsis.com/b2b/?utm_source=radar-site&utm_medium=docs&utm_campaign=radar-base)
