#include "model/time_model.h"

TimeModel::TimeModel() 
  : currentHour(0), currentMinute(0), currentSecond(0),
    lastUpdateMillis(0), lastSecondMillis(0), timeChanged(false) {
}

void TimeModel::init() {
  // Initialize with current system time
  // TODO: Later sync with NTP or RTC
  unsigned long uptimeSeconds = millis() / 1000;
  
  // For now, use a default time (can be set via setTime() or NTP later)
  // Using 12:00:00 as default
  currentHour = 12;
  currentMinute = 0;
  currentSecond = 0;
  
  lastUpdateMillis = millis();
  lastSecondMillis = millis();
  timeChanged = true;
}

void TimeModel::update() {
  unsigned long now = millis();
  
  // Update every second
  if (now - lastSecondMillis >= 1000) {
    lastSecondMillis = now;
    
    // Increment seconds
    currentSecond++;
    
    if (currentSecond >= 60) {
      currentSecond = 0;
      currentMinute++;
      
      if (currentMinute >= 60) {
        currentMinute = 0;
        currentHour++;
        
        if (currentHour >= 24) {
          currentHour = 0;
        }
      }
    }
    
    timeChanged = true;
  }
  
  lastUpdateMillis = now;
}

bool TimeModel::hasTimeChanged() {
  if (timeChanged) {
    timeChanged = false;
    return true;
  }
  return false;
}

void TimeModel::setTime(uint8_t hour, uint8_t minute, uint8_t second) {
  if (hour < 24 && minute < 60 && second < 60) {
    currentHour = hour;
    currentMinute = minute;
    currentSecond = second;
    timeChanged = true;
    lastSecondMillis = millis();
  }
}

void TimeModel::updateFromSystem() {
  // Placeholder for future NTP/RTC integration
  // Currently time is updated via update() method
}

