# Embedded Programmierung - Best Practices

## 🎯 Grundprinzipien

### 1. **Inkrementelle Entwicklung**
✅ **Richtig**: Eine Komponente nach der anderen testen
❌ **Falsch**: Alles auf einmal implementieren

**Beispiel**:
```cpp
// Schritt 1: Nur Serial Monitor testen
void setup() {
  Serial.begin(115200);
  Serial.println("System startet...");
}

// Schritt 2: Display hinzufügen
void setup() {
  Serial.begin(115200);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  Serial.println("Display OK");
}

// Schritt 3: Button hinzufügen
// usw.
```

---

### 2. **Defensive Programmierung**
✅ **Richtig**: Fehlerbehandlung und Validierung
❌ **Falsch**: Annahmen treffen, dass alles funktioniert

**Beispiel**:
```cpp
// ❌ SCHLECHT
void initDisplay() {
  tft.init();  // Was wenn es fehlschlägt?
  tft.fillScreen(TFT_BLACK);
}

// ✅ GUT
bool initDisplay() {
  tft.init();
  if (!tft.isDisplay()) {
    Serial.println("ERROR: Display initialisierung fehlgeschlagen!");
    return false;
  }
  tft.fillScreen(TFT_BLACK);
  Serial.println("Display initialisiert");
  return true;
}

void setup() {
  if (!initDisplay()) {
    // Fehlerbehandlung
    while(1) { delay(1000); } // Stoppe bei Fehler
  }
}
```

---

### 3. **Non-Blocking Code**
✅ **Richtig**: Keine `delay()` in der Hauptschleife
❌ **Falsch**: Lange `delay()` blockieren das System

**Beispiel**:
```cpp
// ❌ SCHLECHT - Blockiert alles
void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(1000);  // System ist 1 Sekunde blockiert!
  digitalWrite(LED_PIN, LOW);
  delay(1000);
  // Button kann nicht gelesen werden während delay()
}

// ✅ GUT - Non-blocking
unsigned long lastBlink = 0;
const unsigned long BLINK_INTERVAL = 1000;

void loop() {
  unsigned long now = millis();
  
  if (now - lastBlink >= BLINK_INTERVAL) {
    static bool ledState = false;
    digitalWrite(LED_PIN, ledState);
    ledState = !ledState;
    lastBlink = now;
  }
  
  // Button kann jederzeit gelesen werden
  checkButton();
}
```

---

### 4. **Zentrale Konfiguration**
✅ **Richtig**: Alle Konstanten in `config.h`
❌ **Falsch**: Magic Numbers im Code

**Beispiel**:
```cpp
// ❌ SCHLECHT
void loop() {
  if (millis() - lastUpdate > 100) {  // Was bedeutet 100?
    updateDisplay();
  }
}

// ✅ GUT
// In config.h:
#define DISPLAY_UPDATE_INTERVAL_MS 100

// In main.cpp:
void loop() {
  if (millis() - lastUpdate > DISPLAY_UPDATE_INTERVAL_MS) {
    updateDisplay();
  }
}
```

---

### 5. **Modularer Code**
✅ **Richtig**: Funktionen für einzelne Aufgaben
❌ **Falsch**: Alles in `setup()` und `loop()`

**Beispiel**:
```cpp
// ✅ GUT - Modular
void initHardware() {
  initDisplay();
  initButtons();
  initBuzzer();
}

void updateDisplay() {
  // Display-Logik
}

void handleInput() {
  // Input-Logik
}

void setup() {
  initHardware();
}

void loop() {
  handleInput();
  updateDisplay();
  delay(10);  // Kurze Pause, damit andere Tasks laufen können
}
```

---

## 🔧 Spezifische Best Practices

### **1. Pin-Management**

```cpp
// ✅ GUT - Alle Pins zentral definiert
// In config.h:
#define BUTTON_PIN 0
#define LED_PIN 14

// In main.cpp:
void initPins() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Pullup aktivieren!
  pinMode(LED_PIN, OUTPUT);
}

// ❌ SCHLECHT - Pins direkt im Code
pinMode(0, INPUT);  // Was ist Pin 0?
```

**Wichtig**: 
- `INPUT_PULLUP` für Buttons verwenden (verhindert Floating)
- Pin-Modi explizit setzen

---

### **2. Timing und Delays**

```cpp
// ✅ GUT - State Machine mit Timestamps
enum State {
  IDLE,
  RUNNING,
  PAUSED
};

State currentState = IDLE;
unsigned long stateStartTime = 0;
unsigned long lastUpdate = 0;

void loop() {
  unsigned long now = millis();
  
  switch(currentState) {
    case IDLE:
      if (buttonPressed()) {
        currentState = RUNNING;
        stateStartTime = now;
      }
      break;
      
    case RUNNING:
      if (now - lastUpdate >= 100) {
        updateTimer();
        lastUpdate = now;
      }
      break;
  }
}

// ❌ SCHLECHT
void loop() {
  delay(100);  // Blockiert alles
  updateTimer();
}
```

**Wichtig**:
- `millis()` für Timing verwenden (läuft weiter, auch bei Überlauf)
- `micros()` für sehr präzise Timing
- Nie `delay()` in der Hauptschleife

---

### **3. Memory Management**

```cpp
// ✅ GUT - Stack-Variablen bevorzugen
void updateDisplay() {
  char buffer[32];  // Stack, automatisch freigegeben
  snprintf(buffer, sizeof(buffer), "Time: %lu", currentTime);
  tft.drawString(buffer, 0, 0);
}

// ⚠️ VORSICHT - Heap-Allokation
void updateDisplay() {
  String text = "Time: " + String(currentTime);  // Heap-Allokation
  // Kann zu Fragmentierung führen
  // Besser: char buffer[] verwenden
}

// ✅ GUT - Für ESP32 mit PSRAM
void setup() {
  if (psramFound()) {
    Serial.println("PSRAM verfügbar!");
    // Kann für große Buffers verwendet werden
  }
}
```

**Wichtig**:
- Stack-Variablen bevorzugen
- `String`-Klasse sparsam verwenden (kann Fragmentierung verursachen)
- PSRAM für große Datenstrukturen nutzen (ESP32-S3 hat 8MB)

---

### **4. Error Handling**

```cpp
// ✅ GUT - Fehlerbehandlung
bool initDisplay() {
  Serial.print("Initialisiere Display... ");
  
  tft.init();
  
  // Test: Zeichne Pixel
  tft.drawPixel(0, 0, TFT_WHITE);
  uint16_t color = tft.readPixel(0, 0);
  
  if (color != TFT_WHITE) {
    Serial.println("FEHLER!");
    return false;
  }
  
  Serial.println("OK");
  return true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  if (!initDisplay()) {
    Serial.println("KRITISCHER FEHLER: Display nicht verfügbar!");
    // Blinkende LED als Fehlersignal
    while(1) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  }
}
```

---

### **5. Debugging**

```cpp
// ✅ GUT - Debug-Makros
#ifdef DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

void loop() {
  DEBUG_PRINT("Button State: ");
  DEBUG_PRINTLN(digitalRead(BUTTON_PIN));
  
  // In platformio.ini: build_flags = -DDEBUG für Debug-Modus
}

// ✅ GUT - Timing-Messung
void updateDisplay() {
  unsigned long start = micros();
  
  // Display-Update-Code
  
  unsigned long duration = micros() - start;
  if (duration > 1000) {  // > 1ms
    DEBUG_PRINTLN("WARNUNG: Display-Update zu langsam!");
  }
}
```

---

### **6. Interrupts (für Buttons)**

```cpp
// ✅ GUT - Interrupt für Button
volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 50;

void IRAM_ATTR buttonISR() {
  buttonPressed = true;
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
}

void loop() {
  if (buttonPressed) {
    unsigned long now = millis();
    
    // Debouncing
    if (now - lastDebounceTime > DEBOUNCE_DELAY) {
      handleButtonPress();
      lastDebounceTime = now;
    }
    
    buttonPressed = false;
  }
}
```

**Wichtig**:
- `volatile` für Interrupt-Variablen
- `IRAM_ATTR` für Interrupt-Handler (muss im RAM sein)
- Debouncing für Buttons

---

## 📋 Code-Organisation

### **Empfohlene Struktur**

```
firmware/
├── src/
│   ├── main.cpp              # setup() und loop()
│   ├── hardware.cpp          # Hardware-Initialisierung
│   ├── display.cpp           # Display-Funktionen
│   ├── input.cpp             # Button/Encoder
│   ├── state_machine.cpp     # State Machine
│   └── timer.cpp             # Timer-Logik
├── include/
│   ├── config.h              # Konfiguration
│   ├── hardware.h
│   ├── display.h
│   ├── input.h
│   ├── state_machine.h
│   └── timer.h
└── platformio.ini
```

### **Beispiel: Modularer Aufbau**

**main.cpp**:
```cpp
#include <Arduino.h>
#include "hardware.h"
#include "state_machine.h"
#include "display.h"
#include "input.h"

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  if (!initHardware()) {
    // Fehlerbehandlung
    while(1) delay(1000);
  }
  
  initStateMachine();
  Serial.println("System bereit");
}

void loop() {
  handleInput();
  updateStateMachine();
  updateDisplay();
  
  delay(10);  // Kurze Pause
}
```

**hardware.h**:
```cpp
#ifndef HARDWARE_H
#define HARDWARE_H

bool initHardware();
void initDisplay();
void initButtons();
void initBuzzer();

#endif
```

---

## 🚨 Häufige Fehler vermeiden

### **1. Floating Inputs**
```cpp
// ❌ SCHLECHT
pinMode(BUTTON_PIN, INPUT);  // Floating, unvorhersehbar

// ✅ GUT
pinMode(BUTTON_PIN, INPUT_PULLUP);  // Pullup aktiviert
```

### **2. millis() Überlauf**
```cpp
// ✅ GUT - Funktioniert auch bei Überlauf
unsigned long now = millis();
if (now - lastTime >= interval) {
  // Funktioniert auch wenn millis() überläuft (nach ~49 Tagen)
}
```

### **3. Blockierende Operationen**
```cpp
// ❌ SCHLECHT
void loop() {
  delay(1000);  // Blockiert alles
  doSomething();
}

// ✅ GUT
unsigned long lastTime = 0;
void loop() {
  if (millis() - lastTime >= 1000) {
    doSomething();
    lastTime = millis();
  }
  // Andere Tasks können laufen
}
```

### **4. Uninitialisierte Variablen**
```cpp
// ❌ SCHLECHT
int counter;  // Uninitialisiert!

// ✅ GUT
int counter = 0;
```

---

## 🎓 Entwicklungs-Workflow

### **1. Planung**
- State Machine designen
- Hardware-Komponenten auflisten
- Pin-Belegung dokumentieren

### **2. Prototyping**
- Eine Komponente nach der anderen
- Jede Komponente einzeln testen
- Serial Monitor für Debugging

### **3. Integration**
- Komponenten zusammenführen
- State Machine implementieren
- Edge Cases testen

### **4. Optimierung**
- Performance messen
- Memory-Usage prüfen
- Code refactoren

### **5. Testing**
- Verschiedene Szenarien testen
- Edge Cases (Button während Timer läuft, etc.)
- Stress-Tests (lange Laufzeit)

---

## 💡 Tipps für dein Chess Clock Projekt

### **1. Timer-Implementierung**
```cpp
// ✅ GUT - Präziser Timer
unsigned long player1Time = 60000;  // 60 Sekunden in ms
unsigned long player2Time = 60000;
unsigned long lastUpdate = 0;

void updateTimer() {
  unsigned long now = millis();
  unsigned long elapsed = now - lastUpdate;
  
  if (currentPlayer == PLAYER_1) {
    if (player1Time > elapsed) {
      player1Time -= elapsed;
    } else {
      player1Time = 0;
      timeUp(PLAYER_1);
    }
  }
  
  lastUpdate = now;
}
```

### **2. Button-Debouncing**
```cpp
// ✅ GUT - Debouncing
unsigned long lastButtonPress = 0;
const unsigned long DEBOUNCE_MS = 50;

bool isButtonPressed() {
  if (digitalRead(BUTTON_PIN) == LOW) {  // Pullup = LOW wenn gedrückt
    unsigned long now = millis();
    if (now - lastButtonPress > DEBOUNCE_MS) {
      lastButtonPress = now;
      return true;
    }
  }
  return false;
}
```

### **3. Display-Updates optimieren**
```cpp
// ✅ GUT - Nur bei Änderung updaten
unsigned long lastDisplayedTime = 0;

void updateDisplay() {
  unsigned long currentTime = getCurrentPlayerTime();
  
  if (currentTime != lastDisplayedTime) {
    // Nur updaten wenn sich Zeit geändert hat
    tft.fillRect(0, 0, 240, 50, TFT_BLACK);
    char buffer[32];
    formatTime(currentTime, buffer);
    tft.drawString(buffer, 120, 25);
    lastDisplayedTime = currentTime;
  }
}
```

---

## 📚 Weiterführende Ressourcen

- **Arduino Reference**: https://www.arduino.cc/reference/
- **ESP32 Documentation**: https://docs.espressif.com/
- **TFT_eSPI Library**: https://github.com/Bodmer/TFT_eSPI
- **PlatformIO Docs**: https://docs.platformio.org/

---

## ✅ Checkliste vor dem Upload

- [ ] Alle Pins in `config.h` definiert
- [ ] Keine `delay()` in `loop()`
- [ ] Fehlerbehandlung implementiert
- [ ] Serial Monitor für Debugging aktiviert
- [ ] Code kommentiert
- [ ] Magic Numbers durch Konstanten ersetzt
- [ ] Button-Debouncing implementiert
- [ ] Display-Updates optimiert (nur bei Änderung)

