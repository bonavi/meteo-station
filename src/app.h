#pragma once

#include <Arduino.h>

#include <ArduinoJson.h>
#include "db.h"
#include "encoder.h"
#include "meteoSensors.h"
#include "display.h"
#include "mqtt.h"

MeteoSensors sensors;
EncoderClass encoder;
Display display;
MQTTClient mqtt_client;

class App {
public:
    // Конструктор принимает все объекты по ссылке
    App(MeteoSensors &sensors,
        EncoderClass &encoder,
        Display &display,
        MQTTClient &mqttClient,
        GyverDBFile &db
    )
        : _meteoSensors(sensors),
          _encoder(encoder),
          _display(display),
          _mqttClient(mqttClient),
          _db(db) {
    }

    void begin() {
        _meteoSensors.begin();
        _display.begin();
        _mqttClient.begin(_db[mqtt_address], _db[mqtt_port].toInt16(), _db[mqtt_user], _db[mqtt_pass]);
    }

    void tick() {
        uint32_t now = millis();

        // Получаем состояние железа
        EncoderEvent encoderState = _encoder.readState();

        // Читаем метео сенсоры
        _meteoSensors.tick();
        _meteoSensors.readMeteoData(now, _sensorData);

        // Подготавливаем состояние дисплея
        _prepareDisplayState(encoderState, now);

        // Рендерим дисплей
        _display.render(_displayState, _sensorData);

        _mqttClient.tick(now);

        if (now - lastMessageSentAt >= 1000) {
            lastMessageSentAt = now;
            if (_buildMeteoDataJSON(payload, sizeof(payload))) {
                _mqttClient.sendMeteoData("esp-meteo-station/01/data", payload);
            }
        }
    }

private:
    MeteoSensorData _sensorData;
    DisplayState _displayState;

    char payload[128];

    uint32_t lastMessageSentAt = 0;

    MeteoSensors &_meteoSensors;
    EncoderClass &_encoder;
    Display &_display;
    MQTTClient &_mqttClient;
    GyverDBFile &_db;

    void _prepareDisplayState(EncoderEvent event, uint32_t now) {
        switch (event) {
            case EncoderEvent::LEFT:
                _displayState.prevScreen();
                break;

            case EncoderEvent::RIGHT:
                _displayState.nextScreen();
                break;

            case EncoderEvent::CLICK:
                _displayState.toggleBacklight();
                break;

            default:
                break;
        }
    }

    bool _buildMeteoDataJSON(
        char *buffer,
        size_t bufferSize
    ) {
        JsonDocument doc;

        doc["t1"] = _sensorData.temperature_1;
        doc["t2"] = _sensorData.temperature_2;
        doc["h"] = _sensorData.humidity;
        doc["p"] = _sensorData.pressure;

        return serializeJson(doc, buffer, bufferSize) > 0;
    }
};
