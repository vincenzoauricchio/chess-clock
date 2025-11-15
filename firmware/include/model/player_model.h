/*
  Player Model for Chess Clock (Model Layer)
  
  This file contains the player data management.
  Handles player name input and storage.
*/

#ifndef PLAYER_MODEL_H
#define PLAYER_MODEL_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/**
 * @brief Maximum length for player name
 */
#define MAX_PLAYER_NAME_LENGTH 32

/**
 * @brief Maximum number of players that can be stored
 */
#define MAX_PLAYERS 50

/**
 * @brief Path to the players JSON file in LittleFS
 */
#define PLAYERS_FILE_PATH "/players.json"

/**
 * @brief Player Model Class
 * 
 * Manages player name input and storage.
 * This is part of the Model layer - it contains no UI logic.
 */
class PlayerModel {
public:
  PlayerModel();
  
  /**
   * @brief Initialize the player model
   */
  void init();
  
  /**
   * @brief Start entering a new player name
   */
  void startEnteringName();
  
  /**
   * @brief Add a character to the current name
   * @param c Character to add
   * @return true if character was added, false if name is full
   */
  bool addCharacter(char c);
  
  /**
   * @brief Remove the last character from the current name
   */
  void removeLastCharacter();
  
  /**
   * @brief Get the current name being entered
   * @return Pointer to the name string
   */
  const char* getCurrentName() const { return currentName; }
  
  /**
   * @brief Get the length of the current name
   * @return Length of the name
   */
  size_t getNameLength() const { return nameLength; }
  
  /**
   * @brief Check if name is empty
   * @return true if name is empty
   */
  bool isNameEmpty() const { return nameLength == 0; }
  
  /**
   * @brief Check if name is full
   * @return true if name has reached maximum length
   */
  bool isNameFull() const { return nameLength >= MAX_PLAYER_NAME_LENGTH - 1; }
  
  /**
   * @brief Save the current name as a player
   * @return true if name was saved successfully
   */
  bool savePlayer();
  
  /**
   * @brief Load all players from LittleFS
   * @return true if players were loaded successfully
   */
  bool loadPlayers();
  
  /**
   * @brief Get the number of stored players
   * @return Number of players
   */
  size_t getPlayerCount() const { return playerCount; }
  
  /**
   * @brief Get a player name by index
   * @param index Player index (0-based)
   * @return Pointer to player name, or nullptr if index is invalid
   */
  const char* getPlayerName(size_t index) const;
  
  /**
   * @brief Check if a player with the given name already exists
   * @param name Player name to check
   * @return true if player exists
   */
  bool playerExists(const char* name) const;
  
  /**
   * @brief Clear the current name
   */
  void clearName();
  
private:
  char currentName[MAX_PLAYER_NAME_LENGTH];
  size_t nameLength;
  
  // Stored players
  char players[MAX_PLAYERS][MAX_PLAYER_NAME_LENGTH];
  size_t playerCount;
  
  /**
   * @brief Initialize LittleFS filesystem
   * @return true if initialization successful
   */
  bool initFilesystem();
  
  /**
   * @brief Read players from JSON file
   * @return true if read successfully
   */
  bool readPlayersFromFile();
  
  /**
   * @brief Write players to JSON file
   * @return true if written successfully
   */
  bool writePlayersToFile();
};

#endif // PLAYER_MODEL_H

