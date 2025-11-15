/*
  Display View for Chess Clock (View Layer)
  
  This file contains the display rendering logic.
  The View layer only handles rendering - no business logic!
*/

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "model/state_machine.h"

/**
 * @brief Display View Class
 * 
 * Handles all display rendering operations.
 * This is part of the View layer - it contains no business logic.
 */
class DisplayView {
public:
  DisplayView();
  
  /**
   * @brief Initialize the display
   * @return true if initialization successful
   */
  bool init();
  
  /**
   * @brief Clear the display
   */
  void clear();
  
  /**
   * @brief Render the idle screen
   */
  void renderIdle();
  
  /**
   * @brief Render the main menu
   * @param selectedIndex Index of the currently selected menu item (0-based)
   */
  void renderMainMenu(int selectedIndex = 0);
  
  /**
   * @brief Render the game mode selection screen
   * @param selectedIndex Index of the currently selected mode (0-based)
   */
  void renderModeSelection(int selectedIndex = 0);
  
  /**
   * @brief Render the player name entry screen with virtual keyboard
   * @param currentName The current name being entered
   * @param selectedKeyIndex Index of the currently selected key (0-based)
   */
  void renderEnterPlayerName(const char* currentName, int selectedKeyIndex);
  
  /**
   * @brief Render the player selection screen
   * @param players Array of player names
   * @param playerCount Number of players
   * @param selectedIndex Index of the currently selected player (0-based)
   * @param title Title to display (e.g., "Select White Player")
   */
  void renderPlayerSelection(const char* players[], size_t playerCount, int selectedIndex, const char* title);
  
  /**
   * @brief Render a specific state
   * @param state The state to render
   */
  void renderState(ChessClockState state);
  
  /**
   * @brief Render game screen with timer information
   * @param player1Time Time remaining for player 1 (in milliseconds)
   * @param player2Time Time remaining for player 2 (in milliseconds)
   * @param currentPlayer Which player's turn it is (true = player 1)
   */
  void renderGame(unsigned long player1Time, unsigned long player2Time, bool currentPlayer);
  
  /**
   * @brief Get the TFT display object (for advanced operations)
   * @return Reference to TFT_eSPI object
   */
  TFT_eSPI& getTFT() { return tft; }
  
private:
  TFT_eSPI tft;
  
  /**
   * @brief Format time from milliseconds to MM:SS string
   * @param timeMs Time in milliseconds
   * @param buffer Output buffer (must be at least 6 bytes)
   */
  void formatTime(unsigned long timeMs, char* buffer);
};

#endif // DISPLAY_H

