#include "model/player_model.h"
#include <string.h>

PlayerModel::PlayerModel()
  : nameLength(0), playerCount(0) {
  currentName[0] = '\0';
  // Initialize players array
  for (size_t i = 0; i < MAX_PLAYERS; i++) {
    players[i][0] = '\0';
  }
}

void PlayerModel::init() {
  clearName();
  
  // Initialize LittleFS
  if (!initFilesystem()) {
    Serial.println("[ERROR]: Failed to initialize LittleFS");
    return;
  }
  
  // Load existing players
  if (!loadPlayers()) {
    Serial.println("[WARN]: Failed to load players, starting with empty list");
    playerCount = 0;
  } else {
    Serial.print("[INFO]: Loaded ");
    Serial.print(playerCount);
    Serial.println(" players from storage");
  }
}

void PlayerModel::startEnteringName() {
  clearName();
}

bool PlayerModel::addCharacter(char c) {
  if (isNameFull()) {
    return false;
  }
  
  currentName[nameLength] = c;
  nameLength++;
  currentName[nameLength] = '\0';
  return true;
}

void PlayerModel::removeLastCharacter() {
  if (nameLength > 0) {
    nameLength--;
    currentName[nameLength] = '\0';
  }
}

bool PlayerModel::savePlayer() {
  // Validate that name is not empty
  if (isNameEmpty()) {
    Serial.println("[ERROR]: Cannot save empty player name");
    return false;
  }
  
  // Check if player already exists
  if (playerExists(currentName)) {
    Serial.print("[WARN]: Player '");
    Serial.print(currentName);
    Serial.println("' already exists, skipping save");
    return false;
  }
  
  // Check if we have space for more players
  if (playerCount >= MAX_PLAYERS) {
    Serial.println("[ERROR]: Maximum number of players reached");
    return false;
  }
  
  // Add player to in-memory list
  strncpy(players[playerCount], currentName, MAX_PLAYER_NAME_LENGTH - 1);
  players[playerCount][MAX_PLAYER_NAME_LENGTH - 1] = '\0';
  playerCount++;
  
  // Save to file
  if (!writePlayersToFile()) {
    Serial.println("[ERROR]: Failed to save players to file");
    // Rollback: remove the player we just added
    playerCount--;
    players[playerCount][0] = '\0';
    return false;
  }
  
  Serial.print("[INFO]: Player '");
  Serial.print(currentName);
  Serial.println("' saved successfully");
  
  return true;
}

bool PlayerModel::loadPlayers() {
  return readPlayersFromFile();
}

const char* PlayerModel::getPlayerName(size_t index) const {
  if (index >= playerCount) {
    return nullptr;
  }
  return players[index];
}

bool PlayerModel::playerExists(const char* name) const {
  if (name == nullptr || strlen(name) == 0) {
    return false;
  }
  
  for (size_t i = 0; i < playerCount; i++) {
    if (strcmp(players[i], name) == 0) {
      return true;
    }
  }
  
  return false;
}

void PlayerModel::clearName() {
  nameLength = 0;
  currentName[0] = '\0';
}

bool PlayerModel::initFilesystem() {
  if (!LittleFS.begin(true)) { // true = format on fail
    Serial.println("[ERROR]: LittleFS mount failed");
    return false;
  }
  
  Serial.println("[INFO]: LittleFS mounted successfully");
  return true;
}

bool PlayerModel::readPlayersFromFile() {
  File file = LittleFS.open(PLAYERS_FILE_PATH, "r");
  if (!file) {
    Serial.println("[INFO]: Players file does not exist, starting fresh");
    playerCount = 0;
    return true; // Not an error, just no file yet
  }
  
  // Get file size
  size_t fileSize = file.size();
  if (fileSize == 0) {
    Serial.println("[INFO]: Players file is empty");
    file.close();
    playerCount = 0;
    return true;
  }
  
  // Allocate buffer for JSON
  char* jsonBuffer = (char*)malloc(fileSize + 1);
  if (jsonBuffer == nullptr) {
    Serial.println("[ERROR]: Failed to allocate memory for JSON");
    file.close();
    return false;
  }
  
  // Read file
  size_t bytesRead = file.readBytes(jsonBuffer, fileSize);
  jsonBuffer[bytesRead] = '\0';
  file.close();
  
  // Parse JSON
  StaticJsonDocument<4096> doc; // Use fixed size for safety (4KB should be enough for 50 players)
  DeserializationError error = deserializeJson(doc, jsonBuffer);
  free(jsonBuffer);
  
  if (error) {
    Serial.print("[ERROR]: Failed to parse JSON: ");
    Serial.println(error.c_str());
    return false;
  }
  
  // Extract players array
  if (!doc.containsKey("players") || !doc["players"].is<JsonArray>()) {
    Serial.println("[ERROR]: Invalid JSON structure: missing 'players' array");
    return false;
  }
  
  JsonArray playersArray = doc["players"];
  playerCount = 0;
  
  for (JsonObject player : playersArray) {
    if (playerCount >= MAX_PLAYERS) {
      Serial.println("[WARN]: Too many players in file, truncating");
      break;
    }
    
    if (player.containsKey("name") && player["name"].is<const char*>()) {
      const char* name = player["name"];
      size_t nameLen = strlen(name);
      if (nameLen > 0 && nameLen < MAX_PLAYER_NAME_LENGTH) {
        strncpy(players[playerCount], name, MAX_PLAYER_NAME_LENGTH - 1);
        players[playerCount][MAX_PLAYER_NAME_LENGTH - 1] = '\0';
        playerCount++;
      }
    }
  }
  
  return true;
}

bool PlayerModel::writePlayersToFile() {
  // Create JSON document (estimate: ~50 bytes per player + overhead)
  StaticJsonDocument<4096> doc; // Use fixed size for safety
  JsonArray playersArray = doc["players"].to<JsonArray>();
  
  // Add all players to JSON array
  for (size_t i = 0; i < playerCount; i++) {
    JsonObject player = playersArray.add<JsonObject>();
    player["name"] = players[i];
  }
  
  // Serialize to string
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Write to file
  File file = LittleFS.open(PLAYERS_FILE_PATH, "w");
  if (!file) {
    Serial.println("[ERROR]: Failed to open players file for writing");
    return false;
  }
  
  size_t bytesWritten = file.print(jsonString);
  file.close();
  
  if (bytesWritten == 0) {
    Serial.println("[ERROR]: Failed to write to players file");
    return false;
  }
  
  return true;
}

