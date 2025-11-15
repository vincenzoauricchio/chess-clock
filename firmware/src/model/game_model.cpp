#include "model/game_model.h"

GameModel::GameModel()
  : currentMode(GameMode::RAPID), whiteTimeMs(0), blackTimeMs(0) {
}

void GameModel::init() {
  // Initialize with default mode (Rapid)
  currentMode = GameMode::RAPID;
  initializePlayerTimes();
}

void GameModel::setGameMode(GameMode mode) {
  currentMode = mode;
  initializePlayerTimes();
}

void GameModel::initializePlayerTimes() {
  switch (currentMode) {
    case GameMode::RAPID:
      whiteTimeMs = RAPID_TIME_MS;
      blackTimeMs = RAPID_TIME_MS;
      break;
    case GameMode::BLITZ:
      whiteTimeMs = BLITZ_TIME_MS;
      blackTimeMs = BLITZ_TIME_MS;
      break;
  }
}

void GameModel::updateWhiteTime(unsigned long elapsedMs) {
  if (whiteTimeMs > elapsedMs) {
    whiteTimeMs -= elapsedMs;
  } else {
    whiteTimeMs = 0; // Time is up
  }
}

void GameModel::updateBlackTime(unsigned long elapsedMs) {
  if (blackTimeMs > elapsedMs) {
    blackTimeMs -= elapsedMs;
  } else {
    blackTimeMs = 0; // Time is up
  }
}

void GameModel::resetGame() {
  initializePlayerTimes();
}

const char* gameModeToString(GameMode mode) {
  switch (mode) {
    case GameMode::RAPID:
      return "Rapid (15 min)";
    case GameMode::BLITZ:
      return "Blitz (5 min)";
    default:
      return "Unknown";
  }
}

