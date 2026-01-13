#pragma once

#include "config.h"
#include <GyverBME280.h>
#include <GyverHTU21D.h>

struct MeteoSensorData {
    float temperature_1;
    float temperature_2;
    float humidity;
    float pressure;
};

class MeteoSensors {
public:
    void begin() {
        _htu.begin();
        _bme.begin(I2C_ADDR_BME280);
    }

    void tick() {
        // Асинхронное чтение HTU21D
        _htu.readTick(MIN_INTERVAL);
    }

    bool readMeteoData(uint32_t now, MeteoSensorData &out) {
        if (now - lastUpdate < MIN_INTERVAL) {
            return false;
        }
        lastUpdate = now;

        out.temperature_1 = _bme.readTemperature();
        out.temperature_2 = _htu.getTemperature();
        out.humidity = _htu.getHumidity();
        out.pressure = _bme.readPressure() / 133.322f;

        return true;
    }

private:
    GyverBME280 _bme;
    GyverHTU21D _htu;
    uint32_t lastUpdate = 0;
    static constexpr uint32_t MIN_INTERVAL = 1000;
};
