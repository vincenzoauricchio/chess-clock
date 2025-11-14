/*
  Configuration file for ESP32 Display Test Project
  
  This file contains WiFi and other system configuration parameters.
  Modify these values according to your network setup.
*/

#ifndef CONFIG_H
#define CONFIG_H

// System Configuration
#define SERIAL_BAUD_RATE 115200             // Serial monitor baud rate

// NFC Configuration
#define NFC_IRQ_PIN         8               // PN532 IRQ pin
#define NFC_RESET_PIN       9               // PN532 Reset pin

// Display Configuration
#define TFT_BACKLIGHT_PIN   1               // TFT backlight pin (PWM capable)

// LED Strip Configuration
#define LED_STRIP_PIN       14              // WS2812B data pin
#define LED_STRIP_COUNT     36              // Number of LEDs in the strip

// Input Configuration
#define ROTARY_PIN_A 5
#define ROTARY_PIN_B 4

#define BUTTON_PIN 0

// Buzzer Configuration
#define BUZZER_PIN 21

#endif // CONFIG_H
