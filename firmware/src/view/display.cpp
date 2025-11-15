#include "view/display.h"
#include "config.h"

DisplayView::DisplayView() {
}

bool DisplayView::init() {
  // Backlight-Pin konfigurieren und aktivieren
  pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(TFT_BACKLIGHT_PIN, HIGH);
  
  // Display initialisieren
  // Note: This might fail if display is not connected
  // The system will continue to work without display for testing
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
  // Set white background
  tft.fillScreen(TFT_WHITE);
  
  tft.setTextDatum(MC_DATUM); // Middle-Center Alignment
  tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  tft.setTextSize(2);
  tft.drawString("Chess Clock", tft.width() / 2, tft.height() / 2 - 20, 2);
  tft.setTextSize(1);
  tft.drawString("Press button to start", tft.width() / 2, tft.height() / 2 + 20, 1);
}

void DisplayView::renderMainMenu(int selectedIndex) {
  // Set white background
  tft.fillScreen(TFT_WHITE);
  
  // Title
  tft.setTextDatum(TC_DATUM); // Top-Center Alignment
  tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  tft.setTextSize(2);
  tft.drawString("Main Menu", tft.width() / 2, 20, 2);
  
  // Menu items
  tft.setTextSize(1);
  int yPos = 60;
  int lineHeight = 30;
  
  // Menu item 0: Play Game
  if (selectedIndex == 0) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("1. Play Game", tft.width() / 2, yPos, 1);
  
  // Menu item 1: Settings
  yPos += lineHeight;
  if (selectedIndex == 1) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("2. Settings", tft.width() / 2, yPos, 1);
}

void DisplayView::renderState(ChessClockState state) {
  switch (state) {
    case ChessClockState::IDLE:
      renderIdle();
      break;
    case ChessClockState::MAIN_MENU:
      // renderMainMenu() is called from StateController with selectedIndex
      // This case should not be reached, but kept for safety
      renderMainMenu(0);
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

