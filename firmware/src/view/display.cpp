#include "view/display.h"
#include "config.h"
#include <string.h>

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
  
  // Menu item 1: Add Player
  yPos += lineHeight;
  if (selectedIndex == 1) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("2. Add Player", tft.width() / 2, yPos, 1);
  
  // Menu item 2: Settings
  yPos += lineHeight;
  if (selectedIndex == 2) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("3. Settings", tft.width() / 2, yPos, 1);
}

void DisplayView::renderModeSelection(int selectedIndex) {
  // Set white background
  tft.fillScreen(TFT_WHITE);
  
  // Title
  tft.setTextDatum(TC_DATUM); // Top-Center Alignment
  tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  tft.setTextSize(2);
  tft.drawString("Select Mode", tft.width() / 2, 20, 2);
  
  // Mode items
  tft.setTextSize(1);
  int yPos = 60;
  int lineHeight = 30;
  
  // Mode item 0: Rapid (Schnellschach - 15 min)
  if (selectedIndex == 0) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("1. Rapid (15 min)", tft.width() / 2, yPos, 1);
  
  // Mode item 1: Blitz (Blitzschach - 5 min)
  yPos += lineHeight;
  if (selectedIndex == 1) {
    // Highlight selected item with background
    tft.fillRect(10, yPos - 5, tft.width() - 20, 25, TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
  } else {
    tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  }
  tft.drawString("2. Blitz (5 min)", tft.width() / 2, yPos, 1);
}

void DisplayView::renderEnterPlayerName(const char* currentName, int selectedKeyIndex) {
  // Set white background
  tft.fillScreen(TFT_WHITE);
  
  // Keyboard layout: 6 columns, 5 rows (last row has only 5 keys)
  // Row 1: A B C D E F (indices 0-5)
  // Row 2: G H I J K L (indices 6-11)
  // Row 3: M N O P Q R (indices 12-17)
  // Row 4: S T U V W X (indices 18-23)
  // Row 5: Y Z Space Del Done (indices 24-28)
  const char keyboard[5][6] = {
    {'A', 'B', 'C', 'D', 'E', 'F'},
    {'G', 'H', 'I', 'J', 'K', 'L'},
    {'M', 'N', 'O', 'P', 'Q', 'R'},
    {'S', 'T', 'U', 'V', 'W', 'X'},
    {'Y', 'Z', ' ', '\b', '\n', '\0'}  // Space, Backspace, Done
  };
  
  const char* specialKeys[5][6] = {
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, "SP", "DEL", "DONE", nullptr}
  };
  
  // Title and current name display area (top)
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.setTextSize(1);
  tft.drawString("Enter Player Name", tft.width() / 2, 5, 1);
  
  // Display current name
  int nameY = 25;
  tft.fillRect(5, nameY - 2, tft.width() - 10, 20, TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  if (strlen(currentName) == 0) {
    tft.drawString("_", tft.width() / 2, nameY + 8, 1);
  } else {
    tft.drawString(currentName, tft.width() / 2, nameY + 8, 1);
  }
  
  // Keyboard area (bottom)
  int keyboardStartY = 55;
  int keyWidth = (tft.width() - 20) / 6;
  int keyHeight = 30;
  int keySpacing = 2;
  
  for (int row = 0; row < 5; row++) {
    int colsInRow = (row == 4) ? 5 : 6; // Last row has only 5 keys
    for (int col = 0; col < colsInRow; col++) {
      int keyIndex = row * 6 + col;
      int x = 10 + col * (keyWidth + keySpacing);
      int y = keyboardStartY + row * (keyHeight + keySpacing);
      
      // Check if this is the selected key
      bool isSelected = (keyIndex == selectedKeyIndex);
      
      // Draw key background
      if (isSelected) {
        tft.fillRect(x, y, keyWidth, keyHeight, TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
      } else {
        tft.fillRect(x, y, keyWidth, keyHeight, TFT_WHITE);
        tft.drawRect(x, y, keyWidth, keyHeight, TFT_BLACK);
        tft.setTextColor(TFT_BLACK, TFT_WHITE);
      }
      
      // Draw key label
      tft.setTextDatum(MC_DATUM);
      tft.setTextSize(1);
      
      // Handle special keys
      if (specialKeys[row][col] != nullptr) {
        tft.drawString(specialKeys[row][col], x + keyWidth / 2, y + keyHeight / 2, 1);
      } else {
        // Regular letter
        char keyChar[2] = {keyboard[row][col], '\0'};
        tft.drawString(keyChar, x + keyWidth / 2, y + keyHeight / 2, 1);
      }
    }
  }
}

void DisplayView::renderPlayerSelection(const char* players[], size_t playerCount, int selectedIndex, const char* title) {
  // Set white background
  tft.fillScreen(TFT_WHITE);
  
  // Title
  tft.setTextDatum(TC_DATUM); // Top-Center Alignment
  tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
  tft.setTextSize(2);
  tft.drawString(title, tft.width() / 2, 5, 2);
  
  // Check if there are any players
  if (playerCount == 0) {
    tft.setTextSize(1);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("No players available", tft.width() / 2, tft.height() / 2, 1);
    tft.drawString("Add players first", tft.width() / 2, tft.height() / 2 + 20, 1);
    return;
  }
  
  // Player list
  tft.setTextSize(1);
  int yPos = 35;
  int lineHeight = 25;
  int maxVisibleItems = (tft.height() - yPos - 10) / lineHeight; // How many items fit on screen
  
  // Calculate scroll offset if needed
  int scrollOffset = 0;
  if (selectedIndex >= maxVisibleItems) {
    scrollOffset = selectedIndex - maxVisibleItems + 1;
  }
  
  // Display visible players
  for (int i = 0; i < (int)playerCount && i < maxVisibleItems; i++) {
    int playerIndex = i + scrollOffset;
    if (playerIndex >= (int)playerCount) {
      break;
    }
    
    int currentY = yPos + i * lineHeight;
    
    // Check if this is the selected player
    bool isSelected = (playerIndex == selectedIndex);
    
    // Draw player item
    if (isSelected) {
      // Highlight selected item with background
      tft.fillRect(10, currentY - 3, tft.width() - 20, lineHeight - 2, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background
    } else {
      tft.setTextColor(TFT_BLACK, TFT_WHITE); // Black text on white background
    }
    
    // Draw player name with index
    char playerText[40];
    snprintf(playerText, sizeof(playerText), "%d. %s", playerIndex + 1, players[playerIndex]);
    tft.drawString(playerText, tft.width() / 2, currentY, 1);
  }
  
  // Show scroll indicator if there are more items
  if (playerCount > maxVisibleItems) {
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    tft.setTextSize(1);
    char scrollText[20];
    snprintf(scrollText, sizeof(scrollText), "%d/%d", selectedIndex + 1, playerCount);
    tft.setTextDatum(BC_DATUM);
    tft.drawString(scrollText, tft.width() / 2, tft.height() - 5, 1);
  }
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
    case ChessClockState::WAIT_FOR_MODE_SELECTION:
      // renderModeSelection() is called from StateController with selectedIndex
      // This case should not be reached, but kept for safety
      renderModeSelection(0);
      break;
    case ChessClockState::ENTER_PLAYER_NAME:
      // renderEnterPlayerName() is called from StateController with name and keyIndex
      // This case should not be reached, but kept for safety
      renderEnterPlayerName("", 0);
      break;
    case ChessClockState::WAIT_FOR_WHITE_PLAYER_SELECTION:
    case ChessClockState::WAIT_FOR_BLACK_PLAYER_SELECTION:
      // renderPlayerSelection() is called from StateController with players array
      // This case should not be reached, but kept for safety
      {
        const char* emptyPlayers[] = {};
        renderPlayerSelection(emptyPlayers, 0, 0, "Select Player");
      }
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

