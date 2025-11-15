/*
  Game Model for Chess Clock (Model Layer)
  
  This file contains the game logic and game data.
  Handles game mode, player times, and game state.
*/

#ifndef GAME_MODEL_H
#define GAME_MODEL_H

#include <Arduino.h>

/**
 * @brief Game mode enumeration
 */
enum class GameMode {
  RAPID,    // Schnellschach - 15 minutes per player
  BLITZ     // Blitzschach - 5 minutes per player
};

/**
 * @brief Game Model Class
 * 
 * Manages game data including:
 * - Game mode (Rapid/Blitz)
 * - Player times (white and black)
 * - Game state
 * 
 * This is part of the Model layer - it contains no UI logic.
 */
class GameModel {
public:
  GameModel();
  
  /**
   * @brief Initialize the game model
   */
  void init();
  
  /**
   * @brief Set the game mode
   * @param mode The game mode to set
   */
  void setGameMode(GameMode mode);
  
  /**
   * @brief Get the current game mode
   * @return Current game mode
   */
  GameMode getGameMode() const { return currentMode; }
  
  /**
   * @brief Initialize player times based on current game mode
   */
  void initializePlayerTimes();
  
  /**
   * @brief Get white player's remaining time (in milliseconds)
   * @return Remaining time in milliseconds
   */
  unsigned long getWhiteTime() const { return whiteTimeMs; }
  
  /**
   * @brief Get black player's remaining time (in milliseconds)
   * @return Remaining time in milliseconds
   */
  unsigned long getBlackTime() const { return blackTimeMs; }
  
  /**
   * @brief Update white player's time (decrement by elapsed time)
   * @param elapsedMs Elapsed time in milliseconds
   */
  void updateWhiteTime(unsigned long elapsedMs);
  
  /**
   * @brief Update black player's time (decrement by elapsed time)
   * @param elapsedMs Elapsed time in milliseconds
   */
  void updateBlackTime(unsigned long elapsedMs);
  
  /**
   * @brief Check if white player's time has run out
   * @return true if time is 0 or less
   */
  bool isWhiteTimeUp() const { return whiteTimeMs == 0; }
  
  /**
   * @brief Check if black player's time has run out
   * @return true if time is 0 or less
   */
  bool isBlackTimeUp() const { return blackTimeMs == 0; }
  
  /**
   * @brief Reset the game (reset times based on current mode)
   */
  void resetGame();
  
private:
  GameMode currentMode;
  unsigned long whiteTimeMs;
  unsigned long blackTimeMs;
  
  static const unsigned long RAPID_TIME_MS = 15 * 60 * 1000;  // 15 minutes
  static const unsigned long BLITZ_TIME_MS = 5 * 60 * 1000;   // 5 minutes
};

/**
 * @brief Get a human-readable string representation of a game mode
 * @param mode The game mode to convert
 * @return const char* String representation
 */
const char* gameModeToString(GameMode mode);

#endif // GAME_MODEL_H

