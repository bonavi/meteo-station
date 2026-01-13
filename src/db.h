#pragma once
#include <GyverDBFile.h>
#include <LittleFS.h>

DB_KEYS(
    kk,
    wifi_ssid,
    wifi_pass,
    mqtt_address,
    mqtt_port,
    mqtt_user,
    mqtt_pass
);

// База данных в файле
GyverDBFile db(&LittleFS, "settings.db");

void db_init() {
    // Инициализируем файловую систему
    LittleFS.begin();

    // Создаем файл
    db.begin();

    // Инициализируем значения по умолчанию
    db.init(kk::wifi_ssid, "");
    db.init(kk::wifi_pass, "");
    db.init(kk::mqtt_address, "mqtt.bonavii.com");
    db.init(kk::mqtt_port, "8883");
    db.init(kk::mqtt_user, "");
    db.init(kk::mqtt_pass, "");
}
