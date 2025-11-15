/*
  Serial Input Handler for Chess Clock (Presenter Layer)
  
  This file provides a way to simulate button/encoder input via Serial Monitor.
  Useful for testing without physical hardware.
*/

#ifndef SERIAL_INPUT_H
#define SERIAL_INPUT_H

#include "presenter/state_controller.h"

/**
 * @brief Serial Input Handler
 * 
 * Reads commands from Serial Monitor and simulates button/encoder input.
 * This allows testing navigation without physical buttons.
 */
class SerialInputHandler {
public:
  SerialInputHandler(StateController& controller);
  
  /**
   * @brief Check for serial input and process commands
   * Should be called in the main loop
   */
  void update();
  
  /**
   * @brief Print available commands to Serial Monitor
   */
  void printHelp();
  
private:
  StateController& controller;
  String inputBuffer;
  
  /**
   * @brief Process a command string
   * @param command The command to process
   */
  void processCommand(const String& command);
};

#endif // SERIAL_INPUT_H

