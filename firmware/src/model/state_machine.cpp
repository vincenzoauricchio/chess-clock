#include "model/state_machine.h"

const char* stateToString(ChessClockState state) {
  switch (state) {
    case ChessClockState::START:
      return "START";
    case ChessClockState::IDLE:
      return "IDLE";
    case ChessClockState::MAIN_MENU:
      return "MAIN_MENU";
    case ChessClockState::WAIT_FOR_MODE_SELECTION:
      return "WAIT_FOR_MODE_SELECTION";
    case ChessClockState::ENTER_PLAYER_NAME:
      return "ENTER_PLAYER_NAME";
    case ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION:
      return "WAIT_FOR_WHITE_PLAYER_SELECTION";
    case ChessClockState::WAIT_FOR_BLACK_PLAYER_SELECTION:
      return "WAIT_FOR_BLACK_PLAYER_SELECTION";
    case ChessClockState::WAIT_FOR_WHITE_START:
      return "WAIT_FOR_WHITE_START";
    case ChessClockState::WHITE_TIME_RUNNING:
      return "WHITE_TIME_RUNNING";
    case ChessClockState::BLACK_TIME_RUNNING:
      return "BLACK_TIME_RUNNING";
    case ChessClockState::PAUSE:
      return "PAUSE";
    case ChessClockState::SAVE_GAME_RESULT:
      return "SAVE_GAME_RESULT";
    default:
      return "UNKNOWN";
  }
}

StateMachineModel::StateMachineModel() : currentState(ChessClockState::START) {
  // Note: State starts as START, but will be initialized to IDLE in controller.init()
}

void StateMachineModel::initializeToIdle() {
  // Direct initialization to IDLE (first state after power-on)
  // This bypasses transition validation as it's the initial boot state
  currentState = ChessClockState::IDLE;
}

bool StateMachineModel::transitionTo(ChessClockState newState) {
  if (isValidTransition(currentState, newState)) {
    currentState = newState;
    return true;
  }
  return false;
}

bool StateMachineModel::isValidTransition(ChessClockState fromState, ChessClockState toState) const {
  // Special case: START -> IDLE is always valid (initial boot transition)
  if (fromState == ChessClockState::START && toState == ChessClockState::IDLE) {
    return true;
  }
  
  // TODO: Implement state transition validation based on state machine diagram
  // For now, allow all other transitions (will be refined later)
  (void)fromState;  // Suppress unused parameter warning
  (void)toState;
  return true;
}

