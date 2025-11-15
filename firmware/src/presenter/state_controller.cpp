#include "presenter/state_controller.h"
#include "model/game_model.h"
#include "model/player_model.h"
#include <Arduino.h>

StateController::StateController(StateMachineModel& model, TimeModel& timeModel, GameModel& gameModel, PlayerModel& playerModel, DisplayView& view)
  : model(model), timeModel(timeModel), gameModel(gameModel), playerModel(playerModel), view(view), 
    lastRenderedState(ChessClockState::START), lastIdleUpdate(0), selectedMenuItem(0), selectedModeIndex(0), selectedKeyIndex(0), selectedPlayerIndex(0) {
}

void StateController::init() {
  // Initialize Time Model
  timeModel.init();
  
  // Initialize Game Model
  gameModel.init();
  
  // Initialize Player Model
  playerModel.init();
  
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
      // Reset menu selection to first item when entering menu
      selectedMenuItem = 0;
      transitionTo(ChessClockState::MAIN_MENU);
      break;
      
    case ChessClockState::MAIN_MENU:
      // Handle menu item selection
      if (selectedMenuItem == 0) {
        // "Play Game" selected
        Serial.println("[INFO]: Play Game selected");
        selectedModeIndex = 0; // Reset to first mode
        transitionTo(ChessClockState::WAIT_FOR_MODE_SELECTION);
      } else if (selectedMenuItem == 1) {
        // "Add Player" selected
        Serial.println("[INFO]: Add Player selected");
        selectedKeyIndex = 0; // Reset to first key
        playerModel.startEnteringName();
        transitionTo(ChessClockState::ENTER_PLAYER_NAME);
      } else if (selectedMenuItem == 2) {
        // "Settings" selected
        Serial.println("[INFO]: Settings selected");
        // TODO: Navigate to settings
      }
      break;
      
    case ChessClockState::ENTER_PLAYER_NAME:
      // Handle keyboard key selection
      // Keyboard layout: 6 columns, 5 rows = 30 keys
      // Keys 0-25: A-Z
      // Key 26: Space
      // Key 27: Backspace (Del)
      // Key 28: Done
      // Key 29: unused (for now)
      
      if (selectedKeyIndex >= 0 && selectedKeyIndex <= 25) {
        // Letter key (A-Z)
        char letter = 'A' + selectedKeyIndex;
        if (playerModel.addCharacter(letter)) {
          Serial.print("[INFO]: Added letter: ");
          Serial.println(letter);
          updateView();
        } else {
          Serial.println("[INFO]: Name is full, cannot add more characters");
        }
      } else if (selectedKeyIndex == 26) {
        // Space key
        if (playerModel.addCharacter(' ')) {
          Serial.println("[INFO]: Added space");
          updateView();
        }
      } else if (selectedKeyIndex == 27) {
        // Backspace key
        playerModel.removeLastCharacter();
        Serial.println("[INFO]: Removed last character");
        updateView();
      } else if (selectedKeyIndex == 28) {
        // Done key - save player and return to menu
        if (playerModel.savePlayer()) {
          Serial.println("[INFO]: Player saved successfully");
          transitionTo(ChessClockState::MAIN_MENU);
        } else {
          Serial.println("[INFO]: Cannot save empty name");
        }
      }
      break;
      
    case ChessClockState::WAIT_FOR_MODE_SELECTION:
      // Handle mode selection
      if (selectedModeIndex == 0) {
        // Rapid (15 min) selected
        Serial.println("[INFO]: Rapid mode (15 min) selected");
        gameModel.setGameMode(GameMode::RAPID);
        selectedPlayerIndex = 0; // Reset to first player
        transitionTo(ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION);
      } else if (selectedModeIndex == 1) {
        // Blitz (5 min) selected
        Serial.println("[INFO]: Blitz mode (5 min) selected");
        gameModel.setGameMode(GameMode::BLITZ);
        selectedPlayerIndex = 0; // Reset to first player
        transitionTo(ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION);
      }
      break;
      
    case ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION:
      // Handle white player selection
      if (playerModel.getPlayerCount() == 0) {
        Serial.println("[WARN]: No players available, cannot start game");
        // TODO: Maybe show error message or go back to menu
        break;
      }
      
      if (selectedPlayerIndex >= 0 && selectedPlayerIndex < (int)playerModel.getPlayerCount()) {
        const char* selectedPlayer = playerModel.getPlayerName(selectedPlayerIndex);
        Serial.print("[INFO]: White player selected: ");
        Serial.println(selectedPlayer);
        // TODO: Store selected white player in GameModel
        selectedPlayerIndex = 0; // Reset for black player selection
        transitionTo(ChessClockState::WAIT_FOR_BLACK_PLAYER_SELECTION);
      }
      break;
      
    // TODO: Add more state transition handlers
    default:
      break;
  }
}

void StateController::handleButtonPress(int buttonId) {
  ChessClockState currentState = model.getCurrentState();
  
  Serial.print("[INFO]: Button ");
  Serial.print(buttonId);
  Serial.print(" pressed in state: ");
  Serial.println(stateToString(currentState));
  
  // Handle button press based on current state and button ID
  switch (currentState) {
    case ChessClockState::WHITE_TIME_RUNNING:
      if (buttonId == 1) {
        // White player presses their button - switch to black
        Serial.println("[INFO]: White pressed - switching to Black");
        transitionTo(ChessClockState::BLACK_TIME_RUNNING);
      }
      break;
      
    case ChessClockState::BLACK_TIME_RUNNING:
      if (buttonId == 2) {
        // Black player presses their button - switch to white
        Serial.println("[INFO]: Black pressed - switching to White");
        transitionTo(ChessClockState::WHITE_TIME_RUNNING);
      }
      break;
      
    case ChessClockState::PAUSE:
      // Either button can resume from pause
      if (buttonId == 1) {
        Serial.println("[INFO]: Resuming - White's turn");
        transitionTo(ChessClockState::WHITE_TIME_RUNNING);
      } else if (buttonId == 2) {
        Serial.println("[INFO]: Resuming - Black's turn");
        transitionTo(ChessClockState::BLACK_TIME_RUNNING);
      }
      break;
      
    // TODO: Add more button handlers for other states
    default:
      Serial.println("[INFO]: Button press not handled in current state");
      break;
  }
}

void StateController::handleEncoderRotation(int direction) {
  ChessClockState currentState = model.getCurrentState();
  
  // Handle encoder rotation based on current state
  switch (currentState) {
    case ChessClockState::MAIN_MENU:
      // Navigate menu items
      selectedMenuItem += direction;
      // Wrap around: if < 0, go to last item; if >= count, go to first item
      if (selectedMenuItem < 0) {
        selectedMenuItem = MENU_ITEM_COUNT - 1;
      } else if (selectedMenuItem >= MENU_ITEM_COUNT) {
        selectedMenuItem = 0;
      }
      // Update view to show new selection
      updateView();
      Serial.print("[INFO]: Menu item selected: ");
      Serial.println(selectedMenuItem);
      break;
      
    case ChessClockState::WAIT_FOR_MODE_SELECTION:
      // Navigate mode selection
      selectedModeIndex += direction;
      // Wrap around: if < 0, go to last mode; if >= count, go to first mode
      if (selectedModeIndex < 0) {
        selectedModeIndex = MODE_COUNT - 1;
      } else if (selectedModeIndex >= MODE_COUNT) {
        selectedModeIndex = 0;
      }
      // Update view to show new selection
      updateView();
      Serial.print("[INFO]: Mode selected: ");
      Serial.println(selectedModeIndex);
      break;
      
    case ChessClockState::ENTER_PLAYER_NAME:
      // Navigate keyboard keys
      selectedKeyIndex += direction;
      // Wrap around: if < 0, go to last key; if >= count, go to first key
      if (selectedKeyIndex < 0) {
        selectedKeyIndex = KEYBOARD_KEY_COUNT - 1;
      } else if (selectedKeyIndex >= KEYBOARD_KEY_COUNT) {
        selectedKeyIndex = 0;
      }
      // Update view to show new selection
      updateView();
      Serial.print("[INFO]: Key selected: ");
      Serial.println(selectedKeyIndex);
      break;
      
    case ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION:
    case ChessClockState::WAIT_FOR_BLACK_PLAYER_SELECTION:
      // Navigate player list
      {
        size_t playerCount = playerModel.getPlayerCount();
        if (playerCount > 0) {
          selectedPlayerIndex += direction;
          // Wrap around: if < 0, go to last player; if >= count, go to first player
          if (selectedPlayerIndex < 0) {
            selectedPlayerIndex = playerCount - 1;
          } else if (selectedPlayerIndex >= (int)playerCount) {
            selectedPlayerIndex = 0;
          }
          // Update view to show new selection
          updateView();
          Serial.print("[INFO]: Player selected: ");
          Serial.println(selectedPlayerIndex);
        }
      }
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
  
  // Pass selection index based on current state
  if (currentState == ChessClockState::MAIN_MENU) {
    view.renderMainMenu(selectedMenuItem);
  } else if (currentState == ChessClockState::WAIT_FOR_MODE_SELECTION) {
    view.renderModeSelection(selectedModeIndex);
  } else if (currentState == ChessClockState::ENTER_PLAYER_NAME) {
    view.renderEnterPlayerName(playerModel.getCurrentName(), selectedKeyIndex);
  } else if (currentState == ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION) {
    // Build players array for rendering
    size_t playerCount = playerModel.getPlayerCount();
    const char* players[MAX_PLAYERS];
    for (size_t i = 0; i < playerCount; i++) {
      players[i] = playerModel.getPlayerName(i);
    }
    view.renderPlayerSelection(players, playerCount, selectedPlayerIndex, "Select White Player");
  } else if (currentState == ChessClockState::WAIT_FOR_BLACK_PLAYER_SELECTION) {
    // Build players array for rendering
    size_t playerCount = playerModel.getPlayerCount();
    const char* players[MAX_PLAYERS];
    for (size_t i = 0; i < playerCount; i++) {
      players[i] = playerModel.getPlayerName(i);
    }
    view.renderPlayerSelection(players, playerCount, selectedPlayerIndex, "Select Black Player");
  } else {
    view.renderState(currentState);
  }
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

