/*
  Time Model for Chess Clock (Model Layer)
  
  This file contains the time management logic.
  Handles current time display for IDLE mode.
*/

#ifndef TIME_MODEL_H
#define TIME_MODEL_H

#include <Arduino.h>

/**
 * @brief Time Model Class
 * 
 * Manages current time for display in IDLE mode.
 * This is part of the Model layer - it contains no UI logic.
 * 
 * Note: Currently uses system time. Can be extended with NTP/RTC later.
 */
class TimeModel {
public:
  TimeModel();
  
  /**
   * @brief Initialize the time model
   * Should be called in setup()
   */
  void init();
  
  /**
   * @brief Update the time (should be called regularly)
   */
  void update();
  
  /**
   * @brief Get current hour (0-23)
   * @return Current hour
   */
  uint8_t getHour() const { return currentHour; }
  
  /**
   * @brief Get current minute (0-59)
   * @return Current minute
   */
  uint8_t getMinute() const { return currentMinute; }
  
  /**
   * @brief Get current second (0-59)
   * @return Current second
   */
  uint8_t getSecond() const { return currentSecond; }
  
  /**
   * @brief Check if time has changed since last check
   * Resets the flag after reading
   * @return true if time has changed
   */
  bool hasTimeChanged();
  
  /**
   * @brief Set time manually (for testing or RTC/NTP sync)
   * @param hour Hour (0-23)
   * @param minute Minute (0-59)
   * @param second Second (0-59)
   */
  void setTime(uint8_t hour, uint8_t minute, uint8_t second);
  
private:
  uint8_t currentHour;
  uint8_t currentMinute;
  uint8_t currentSecond;
  
  unsigned long lastUpdateMillis;
  unsigned long lastSecondMillis;
  bool timeChanged;
  
  /**
   * @brief Update time from system (millis since boot)
   * This is a placeholder - can be replaced with NTP/RTC later
   */
  void updateFromSystem();
};

#endif // TIME_MODEL_H

