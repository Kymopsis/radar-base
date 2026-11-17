# Kymopsis Radar Base firmware (open edition).
set(srcs "main.c" "radar_link.c" "web.c" "logbuf.c" "spi_rx.c" "adc_format.c" "radar_cfg.c" "usb_fwd.c" "app_json.c" "wifi.c" "mqtt_ha.c" "board.c")
set(incs ".")
set(embed "")

idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS ${incs}
                       EMBED_TXTFILES "index.html" "app.html" "i18n/zh-CN.json" "i18n/zh-TW.json" "i18n/ja.json" "i18n/ko.json" "i18n/de.json" "i18n/fr.json" "i18n/es.json" "i18n/pt.json" "i18n/it.json" "i18n/ru.json" "../../pc-gui/assets/kymopsis-logo-artistic-v4-light.svg"
                       EMBED_FILES ${embed}
                       REQUIRES esp_timer driver esp_driver_gpio esp_driver_uart esp_driver_spi esp_wifi esp_netif esp_event nvs_flash
                                esp_http_server spiffs esp_driver_usb_serial_jtag vfs esp_security mbedtls)

