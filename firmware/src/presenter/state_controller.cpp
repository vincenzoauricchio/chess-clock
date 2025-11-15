#include "presenter/state_controller.h"
#include <Arduino.h>

StateController::StateController(StateMachineModel& model, TimeModel& timeModel, DisplayView& view)
  : model(model), timeModel(timeModel), view(view), 
    lastRenderedState(ChessClockState::START), lastIdleUpdate(0) {
}

void StateController::init() {
  // Initialize Time Model
  timeModel.init();
  
  // Initialize to IDLE state (first state after power-on)
  // This is always the first state the system enters after boot
  model.initializeToIdle();
  lastRenderedState = ChessClockState::IDLE;
  updateView();
  
  Serial.println("System initialized to IDLE state");
}

void StateController::update() {
  // Update time model (for future use)
  timeModel.update();
  
  // Check if state has changed and update view if necessary
  ChessClockState currentState = model.getCurrentState();
  if (currentState != lastRenderedState) {
    updateView();
    lastRenderedState = currentState;
  }
}

void StateController::handleButtonPress() {
  ChessClockState currentState = model.getCurrentState();
  
  // Handle state transitions based on current state
  switch (currentState) {
    case ChessClockState::IDLE:
      transitionTo(ChessClockState::MAIN_MENU);
      break;
      
    case ChessClockState::MAIN_MENU:
      // TODO: Handle menu selection
      break;
      
    // TODO: Add more state transition handlers
    default:
      break;
  }
}

void StateController::handleEncoderRotation(int direction) {
  ChessClockState currentState = model.getCurrentState();
  
  // Handle encoder rotation based on current state
  switch (currentState) {
    case ChessClockState::MAIN_MENU:
      // TODO: Navigate menu items
      break;
      
    // TODO: Add more encoder handlers
    default:
      break;
  }
}

ChessClockState StateController::getCurrentState() const {
  return model.getCurrentState();
}

void StateController::updateView() {
  ChessClockState currentState = model.getCurrentState();
  view.renderState(currentState);
}

void StateController::transitionTo(ChessClockState newState) {
  if (model.transitionTo(newState)) {
    updateView();
    lastRenderedState = newState;
    
    // Debug output
    Serial.print("State transitioned to: ");
    Serial.println(stateToString(newState));
  }
}

