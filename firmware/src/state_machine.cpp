#include "state_machine.h"

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

