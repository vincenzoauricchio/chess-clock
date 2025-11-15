#include "presenter/serial_input.h"
#include <Arduino.h>

SerialInputHandler::SerialInputHandler(StateController& controller)
  : controller(controller) {
}

void SerialInputHandler::update() {
  // Read available characters from Serial
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    // Handle newline or carriage return as command terminator
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        processCommand(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
    }
  }
}

void SerialInputHandler::processCommand(const String& command) {
  String cmd = command;
  cmd.toLowerCase();
  cmd.trim();
  
  // Rotary Encoder Button Press
  if (cmd == "p" || cmd == "press" || cmd == "rotary") {
    Serial.println("[CMD]: Rotary encoder button pressed");
    controller.handleButtonPress();
  }
  // Individual Button 1 (e.g., White/Player 1)
  else if (cmd == "1" || cmd == "b1" || cmd == "button1" || cmd == "w" || cmd == "white") {
    Serial.println("[CMD]: Button 1 pressed (White/Player 1)");
    controller.handleButtonPress(1);
  }
  // Individual Button 2 (e.g., Black/Player 2)
  else if (cmd == "2" || cmd == "b2" || cmd == "button2" || cmd == "bl" || cmd == "black") {
    Serial.println("[CMD]: Button 2 pressed (Black/Player 2)");
    controller.handleButtonPress(2);
  }
  // Rotary Encoder: Rotate clockwise (forward/right)
  else if (cmd == "r" || cmd == "right" || cmd == "forward" || cmd == "cw" || 
           cmd == "e+" || cmd == "encoder+" || cmd == "+" || cmd == ">") {
    Serial.println("[CMD]: Rotary encoder: Rotate clockwise (forward)");
    controller.handleEncoderRotation(1);
  }
  // Rotary Encoder: Rotate counter-clockwise (backward/left)
  else if (cmd == "l" || cmd == "left" || cmd == "back" || cmd == "backward" || 
           cmd == "ccw" || cmd == "e-" || cmd == "encoder-" || cmd == "-" || cmd == "<") {
    Serial.println("[CMD]: Rotary encoder: Rotate counter-clockwise (backward)");
    controller.handleEncoderRotation(-1);
  }
  // Help
  else if (cmd == "h" || cmd == "help") {
    printHelp();
  }
  // Show current state
  else if (cmd == "s" || cmd == "state") {
    Serial.print("[INFO]: Current state: ");
    Serial.println(stateToString(controller.getCurrentState()));
  }
  else {
    Serial.print("[ERROR]: Unknown command: ");
    Serial.println(command);
    Serial.println("[INFO]: Type 'help' or 'h' for available commands");
  }
}

void SerialInputHandler::printHelp() {
  Serial.println("\n=== Chess Clock - Input Simulation ===");
  Serial.println("BUTTONS:");
  Serial.println("  p, press, rotary     - Press rotary encoder button");
  Serial.println("  1, b1, w, white      - Press Button 1 (White/Player 1)");
  Serial.println("  2, b2, bl, black     - Press Button 2 (Black/Player 2)");
  Serial.println("");
  Serial.println("ROTARY ENCODER:");
  Serial.println("  r, right, +, >      - Rotate clockwise (forward)");
  Serial.println("  l, left, -, <       - Rotate counter-clockwise (backward)");
  Serial.println("");
  Serial.println("OTHER COMMANDS:");
  Serial.println("  s, state            - Show current state");
  Serial.println("  h, help             - Show this help");
  Serial.println("");
  Serial.println("QUICK REFERENCE:");
  Serial.println("  p = rotary button");
  Serial.println("  1 = button 1 (white)");
  Serial.println("  2 = button 2 (black)");
  Serial.println("  r = rotate right");
  Serial.println("  l = rotate left");
  Serial.println("=====================================\n");
}

