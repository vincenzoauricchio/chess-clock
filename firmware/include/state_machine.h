/*
  State Machine Definition for Chess Clock
  
  This file defines all states of the chess clock state machine
  based on the state machine diagram.
*/

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

/**
 * @brief Enumeration of all possible states in the chess clock state machine
 * 
 * States are ordered according to the typical flow:
 * 1. Initial states (START, IDLE)
 * 2. Menu states (MAIN_MENU, etc.)
 * 3. Game setup states (player selection, etc.)
 * 4. Game running states (timer running, pause)
 * 5. Game end states (save result)
 */
enum class ChessClockState {
  // Initial States
  START,                          // Initial state (double circle in diagram)
  IDLE,                           // Idle state - shows current time
  
  // Menu States
  MAIN_MENU,                      // Main menu
  WAIT_FOR_MODE_SELECTION,        // Waiting for mode selection
  ENTER_PLAYER_NAME,              // Enter first and last name (for new player)
  
  // Game Setup States
  WAIT_FOR_WHITE_PLAYER_SELECTION,    // Waiting for white player selection
  WAIT_FOR_BLACK_PLAYER_SELECTION,  // Waiting for black player selection
  WAIT_FOR_WHITE_START,           // Waiting for start of white's time
  
  // Game Running States
  WHITE_TIME_RUNNING,             // White's time is running
  BLACK_TIME_RUNNING,             // Black's time is running
  PAUSE,                          // Game is paused
  
  // Game End States
  SAVE_GAME_RESULT                // Save game result
};

/**
 * @brief Get a human-readable string representation of a state
 * 
 * @param state The state to convert
 * @return const char* String representation of the state
 */
const char* stateToString(ChessClockState state);

#endif // STATE_MACHINE_H

