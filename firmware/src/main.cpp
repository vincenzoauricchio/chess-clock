#include <Arduino.h>
#include "config.h"
#include "model/state_machine.h"
#include "model/time_model.h"
#include "view/display.h"
#include "presenter/state_controller.h"
#include "presenter/serial_input.h"

// MVP Components
StateMachineModel stateMachine;
TimeModel timeModel;
DisplayView display;
StateController controller(stateMachine, timeModel, display);
SerialInputHandler serialInput(controller);

void setup() {
  // Serial Monitor initialisieren - SO FRÜH WIE MÖGLICH
  Serial.begin(SERIAL_BAUD_RATE);
  
  // Warte auf Serial-Verbindung
  // ESP32-S3 mit USB CDC braucht etwas Zeit
  delay(3000);  // Längere Verzögerung für stabilere Verbindung
  
  // Test-Ausgabe sofort
  Serial.println("\n\n");
  Serial.println("[INFO]: ========================================");
  Serial.println("[INFO]: Chess Clock - System Starting");
  Serial.println("[INFO]: ========================================");
  Serial.flush();  // Stelle sicher, dass Daten gesendet werden
  
  // Initialize View (Display) - mit Timeout
  Serial.println("[INFO]: Initializing display...");
  Serial.flush();
  
  unsigned long displayStart = millis();
  bool displayOk = display.init();
  unsigned long displayTime = millis() - displayStart;
  
  if (!displayOk) {
    Serial.print("[ERROR]: Display initialization failed! (took ");
    Serial.print(displayTime);
    Serial.println("ms)");
    Serial.println("[INFO]: System will continue without display");
  } else {
    Serial.print("[INFO]: Display initialized successfully (took ");
    Serial.print(displayTime);
    Serial.println("ms)");
  }
  Serial.flush();
  
  // Initialize Controller (which initializes Model)
  Serial.println("[INFO]: Initializing state controller...");
  Serial.flush();
  controller.init();
  Serial.println("[INFO]: State Controller initialized");
  Serial.flush();
  
  Serial.println("\n[INFO]: ========================================");
  Serial.println("[INFO]: System ready!");
  Serial.println("[INFO]: ========================================\n");
  Serial.flush();
  
  // Print help for serial commands
  serialInput.printHelp();
  Serial.flush();
}

void loop() {
  // Update state machine and view
  controller.update();
  
  // Check for serial input (simulates button/encoder)
  serialInput.update();
  
  delay(10);
}
