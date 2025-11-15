#include <Arduino.h>
#include "config.h"
#include "model/state_machine.h"
#include "model/time_model.h"
#include "view/display.h"
#include "presenter/state_controller.h"

// MVP Components
StateMachineModel stateMachine;
TimeModel timeModel;
DisplayView display;
StateController controller(stateMachine, timeModel, display);

void setup() {
  // Serial Monitor initialisieren
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println("Chess Clock - MVP Architecture");
  
  // Initialize View (Display)
  if (!display.init()) {
    Serial.println("ERROR: Display initialization failed!");
    while(1) delay(1000); // Stop on error
  }
  Serial.println("Display initialized");
  
  // Initialize Controller (which initializes Model)
  controller.init();
  Serial.println("State Controller initialized");
  
  Serial.println("System ready!");
}

void loop() {
  // Update state machine and view
  controller.update();
  
  // TODO: Read input (button, encoder) and pass to controller
  // For now, just a simple delay
  delay(10);
}
