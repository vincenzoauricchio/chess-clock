#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "state_machine.h"

// Display-Objekt erstellen
TFT_eSPI tft = TFT_eSPI();

// State Machine
ChessClockState currentState = ChessClockState::START;

void setup() {
  // Serial Monitor initialisieren
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  Serial.println("Chess Clock - Display Test");

  // Backlight-Pin konfigurieren und aktivieren
  pinMode(TFT_BACKLIGHT_PIN, OUTPUT);
  digitalWrite(TFT_BACKLIGHT_PIN, HIGH);

  // Display initialisieren
  tft.init();
  tft.setRotation(1); // Landscape-Modus (kann 0-3 sein, je nach Ausrichtung)
  
  // Display löschen und Hintergrundfarbe setzen
  tft.fillScreen(TFT_BLACK);
  
  // Textfarbe und -größe setzen
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  
  // "Hello World" in der Mitte des Displays ausgeben
  tft.setTextDatum(MC_DATUM); // Middle-Center Alignment
  tft.drawString("Hello Vincenzo!", tft.width() / 2, tft.height() / 2, 2);
  
  Serial.println("Display initialized - Hello World displayed");
  
  // State Machine initialisieren
  currentState = ChessClockState::IDLE;
  Serial.print("State Machine initialized: ");
  Serial.println(stateToString(currentState));
}

void loop() {
  // Hier könnte später die State Machine laufen
  delay(100);
}