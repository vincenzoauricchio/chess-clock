/*
  State Controller for Chess Clock (Presenter Layer)
  
  This file contains the presenter logic that connects Model and View.
  The Presenter handles state transitions and coordinates updates.
*/

#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "model/state_machine.h"
#include "model/time_model.h"
#include "model/game_model.h"
#include "model/player_model.h"
#include "view/display.h"

/**
 * @brief State Controller (Presenter)
 * 
 * Coordinates between Model (State Machine, Time) and View (Display).
 * Handles state transitions and updates the view accordingly.
 */
class StateController {
public:
  StateController(StateMachineModel& model, TimeModel& timeModel, GameModel& gameModel, PlayerModel& playerModel, DisplayView& view);
  
  /**
   * @brief Initialize the controller
   */
  void init();
  
  /**
   * @brief Update the state machine and view
   * Should be called in the main loop
   */
  void update();
  
  /**
   * @brief Handle button press event (rotary encoder button)
   */
  void handleButtonPress();
  
  /**
   * @brief Handle individual button press
   * @param buttonId Button identifier (1 = Button 1, 2 = Button 2)
   */
  void handleButtonPress(int buttonId);
  
  /**
   * @brief Handle encoder rotation
   * @param direction 1 for clockwise, -1 for counterclockwise
   */
  void handleEncoderRotation(int direction);
  
  /**
   * @brief Get the current state
   * @return Current state
   */
  ChessClockState getCurrentState() const;
  
private:
  StateMachineModel& model;
  TimeModel& timeModel;
  GameModel& gameModel;
  PlayerModel& playerModel;
  DisplayView& view;
  ChessClockState lastRenderedState;
  unsigned long lastIdleUpdate;
  int selectedMenuItem; // Current menu selection index
  int selectedModeIndex; // Current mode selection index (0 = Rapid, 1 = Blitz)
  int selectedKeyIndex; // Current keyboard key selection index (0-28)
  int selectedPlayerIndex; // Current player selection index (0-based)
  static const int MENU_ITEM_COUNT = 3; // Number of menu items (Play Game, Add Player, Settings)
  static const int MODE_COUNT = 2; // Number of game modes
  static const int KEYBOARD_KEY_COUNT = 29; // 26 letters (A-Z) + Space + Del + Done
  
  /**
   * @brief Update the view based on current state
   */
  void updateView();
  
  /**
   * @brief Handle state transition
   * @param newState The state to transition to
   */
  void transitionTo(ChessClockState newState);
};

#endif // STATE_CONTROLLER_H

