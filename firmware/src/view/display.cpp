#include "view/display.h"
#include "config.h"

DisplayView::DisplayView() {
}

bool DisplayView::init() {
  // Backlight-Pin konfigurieren und aktivieren
  pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(TFT_BACKLIGHT_PIN, HIGH);
  
  // Display initialisieren
  tft.init();
  tft.setRotation(1); // Landscape-Modus
  
  // Display löschen und Hintergrundfarbe setzen
  tft.fillScreen(TFT_BLACK);
  
  // Textfarbe und -größe setzen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  
  return true;
}

void DisplayView::clear() {
  tft.fillScreen(TFT_BLACK);
}

void DisplayView::renderIdle() {
  clear();
  tft.setTextDatum(MC_DATUM); // Middle-Center Alignment
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Chess Clock", tft.width() / 2, tft.height() / 2 - 20, 2);
  tft.setTextSize(1);
  tft.drawString("Press button to start", tft.width() / 2, tft.height() / 2 + 20, 1);
}

void DisplayView::renderMainMenu() {
  clear();
  tft.setTextDatum(TC_DATUM); // Top-Center Alignment
  tft.drawString("Main Menu", tft.width() / 2, 20, 2);
  tft.drawString("1. Play Game", tft.width() / 2, 60, 1);
  tft.drawString("2. Settings", tft.width() / 2, 80, 1);
}

void DisplayView::renderState(ChessClockState state) {
  switch (state) {
    case ChessClockState::IDLE:
      renderMainMenu();
      break;
    case ChessClockState::MAIN_MENU:
      renderMainMenu();
      break;
    // TODO: Add more state renderings
    default:
      clear();
      tft.setTextDatum(MC_DATUM);
      tft.drawString(stateToString(state), tft.width() / 2, tft.height() / 2, 1);
      break;
  }
}

void DisplayView::renderGame(unsigned long player1Time, unsigned long player2Time, bool currentPlayer) {
  clear();
  
  char timeBuffer[10];
  
  // Player 1 (White) - Top
  tft.setTextColor(currentPlayer ? TFT_GREEN : TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("White", tft.width() / 2, 10, 2);
  formatTime(player1Time, timeBuffer);
  tft.drawString(timeBuffer, tft.width() / 2, 40, 3);
  
  // Player 2 (Black) - Bottom
  tft.setTextColor(!currentPlayer ? TFT_GREEN : TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(BC_DATUM);
  tft.drawString("Black", tft.width() / 2, tft.height() - 10, 2);
  formatTime(player2Time, timeBuffer);
  tft.drawString(timeBuffer, tft.width() / 2, tft.height() - 50, 3);
}

void DisplayView::formatTime(unsigned long timeMs, char* buffer) {
  unsigned long totalSeconds = timeMs / 1000;
  unsigned long minutes = totalSeconds / 60;
  unsigned long seconds = totalSeconds % 60;
  
  snprintf(buffer, 10, "%02lu:%02lu", minutes, seconds);
}

