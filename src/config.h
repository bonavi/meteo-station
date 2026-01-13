#pragma once

#include <Arduino.h>

// Пины энкодера
#if defined(ESP8266)
#define PIN_ENCODER_A      14
#define PIN_ENCODER_B      12
#define PIN_ENCODER_BTN    13
#elif defined(ARDUINO_AVR_NANO)
#define PIN_ENCODER_BTN    2
#define PIN_ENCODER_A      3
#define PIN_ENCODER_B      4
#endif

// I2C адреса
#define I2C_ADDR_BME280   0x76

// LCD размеры
constexpr uint8_t LCD_COLS = 16;
constexpr uint8_t LCD_ROWS = 2;