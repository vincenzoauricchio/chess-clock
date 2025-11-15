# Projektstruktur - Chess Clock

## 📁 Verzeichnisstruktur

```
chess-clock/
├── README.md                          # Projekt-Dokumentation
├── chess-clock-state-machine.v1.drawio # State Machine Diagramm (Quelldatei)
├── chess-clock-state-machine.v1.png    # State Machine Diagramm (Bild)
└── firmware/                           # Hauptverzeichnis für Embedded-Code
    ├── platformio.ini                  # Build-Konfiguration (wichtig!)
    ├── boards/                         # Board-spezifische Definitionen
    │   └── esp32-s3-devkitc-1-n16r8v.json
    ├── include/                        # Header-Dateien (.h)
    │   └── config.h
    └── src/                            # Quellcode-Dateien (.cpp)
        └── main.cpp
```

---

## 📄 Dateien im Detail

### **Root-Verzeichnis**

#### `README.md`
- **Zweck**: Projekt-Dokumentation
- **Inhalt**: Beschreibung des Projekts, Features, State Machine Diagramm
- **Für wen**: Entwickler, die das Projekt verstehen wollen

#### `chess-clock-state-machine.v1.drawio` & `.png`
- **Zweck**: Visualisierung der State Machine
- **Inhalt**: Diagramm der verschiedenen Zustände der Schachuhr
- **Für wen**: Entwickler, die die Logik verstehen wollen

---

### **firmware/** - Das Herzstück des Projekts

#### `platformio.ini` ⚙️
**Die wichtigste Konfigurationsdatei!**

- **Zweck**: Definiert, wie das Projekt kompiliert und auf den Mikrocontroller geladen wird
- **Wichtige Einstellungen**:
  - **Board**: Welches Hardware-Board verwendet wird (ESP32-S3)
  - **Framework**: Arduino Framework (vereinfacht die Programmierung)
  - **Libraries**: Welche externen Bibliotheken benötigt werden
  - **Build Flags**: Compiler-Optionen (z.B. Pin-Belegungen für Display)
  - **Upload Speed**: Wie schnell der Code hochgeladen wird

**Analogie**: Wie eine `package.json` in Node.js oder `requirements.txt` in Python

#### `src/main.cpp` 💻
**Dein Hauptprogramm!**

- **Zweck**: Enthält den ausführbaren Code
- **Struktur**:
  - `#include`-Direktiven: Importiert benötigte Libraries
  - `setup()`: Wird **einmal** beim Start ausgeführt (Initialisierung)
  - `loop()`: Wird **kontinuierlich** wiederholt (Hauptlogik)

**Analogie**: Wie `main()` in C/C++ oder `if __name__ == "__main__"` in Python

**Beispiel-Struktur**:
```cpp
void setup() {
  // Hardware initialisieren
  // Pins konfigurieren
  // Display starten
}

void loop() {
  // State Machine
  // Input lesen
  // Display aktualisieren
  // Timer verwalten
}
```

#### `include/config.h` 🔧
**Konfigurationsdatei für Hardware-Pins**

- **Zweck**: Zentrale Definition aller Pin-Nummern und Konstanten
- **Vorteile**:
  - Änderungen an einem Ort
  - Bessere Lesbarkeit
  - Keine "magischen Zahlen" im Code

**Beispiel**:
```cpp
#define BUTTON_PIN 0        // Statt überall im Code "0" zu schreiben
#define LED_STRIP_PIN 14    // schreibt man hier "LED_STRIP_PIN"
```

**Analogie**: Wie eine `.env`-Datei oder Konfigurationsdatei

#### `boards/esp32-s3-devkitc-1-n16r8v.json` 🎯
**Board-Definition**

- **Zweck**: Beschreibt die technischen Spezifikationen des ESP32-S3 Boards
- **Inhalt**:
  - CPU-Geschwindigkeit (240 MHz)
  - Flash-Größe (16 MB)
  - PSRAM-Größe (8 MB)
  - Upload-Parameter
- **Wird verwendet von**: PlatformIO, um den Code richtig zu kompilieren

**Normalerweise nicht selbst bearbeiten!**

---

## 🔄 Workflow: Wie funktioniert das alles zusammen?

### 1. **Schreiben** (`main.cpp`)
Du schreibst deinen Code in `main.cpp`

### 2. **Konfigurieren** (`config.h`, `platformio.ini`)
- Pins in `config.h` definieren
- Build-Optionen in `platformio.ini` setzen

### 3. **Kompilieren** (PlatformIO)
```bash
pio run
```
- PlatformIO liest `platformio.ini`
- Lädt benötigte Libraries
- Kompiliert `main.cpp` mit den Build-Flags
- Erstellt eine `.bin`-Datei

### 4. **Hochladen** (PlatformIO)
```bash
pio run -t upload
```
- Die `.bin`-Datei wird auf den ESP32-S3 geladen
- Der Mikrocontroller startet automatisch

### 5. **Ausführen**
- `setup()` läuft einmal
- `loop()` läuft endlos

---

## 🎓 Embedded-Programmierung Konzepte

### **setup() vs. loop()**
- **setup()**: Einmalige Initialisierung (wie `__init__` in Python)
- **loop()**: Endlosschleife (wie `while True:` in Python)

### **Pins**
- Mikrocontroller haben physische Pins (GPIO = General Purpose Input/Output)
- Pins können als INPUT oder OUTPUT konfiguriert werden
- Beispiel: Pin 0 = Button, Pin 14 = LED Strip

### **Libraries**
- Vorgefertigter Code für Hardware-Komponenten
- Beispiel: `TFT_eSPI` für Display-Steuerung
- Werden in `platformio.ini` definiert

### **Build Flags**
- Compiler-Direktiven, die während der Kompilierung gesetzt werden
- Beispiel: `-DTFT_CS=10` bedeutet: "Definiere TFT_CS als 10"
- Werden von Libraries gelesen (z.B. TFT_eSPI)

---

## 📚 Typische Erweiterungen

Wenn das Projekt wächst, könntest du hinzufügen:

```
firmware/
├── src/
│   ├── main.cpp           # Hauptprogramm
│   ├── state_machine.cpp  # State Machine Logik
│   ├── display.cpp        # Display-Funktionen
│   └── input.cpp          # Button/Encoder Handling
├── include/
│   ├── config.h
│   ├── state_machine.h    # Header für State Machine
│   ├── display.h
│   └── input.h
└── lib/                   # Eigene Libraries (optional)
    └── custom_library/
```

---

## 🚀 Nächste Schritte

1. **Code in `main.cpp` schreiben**
2. **Pins in `config.h` anpassen** (falls Hardware geändert wird)
3. **Libraries in `platformio.ini` hinzufügen** (falls neue Hardware)
4. **Kompilieren und testen**

---

## 💡 Tipps für Anfänger

1. **Ein Fehler an einem Ort**: Wenn ein Pin nicht funktioniert, prüfe:
   - `config.h` (Pin-Nummer)
   - `platformio.ini` (Build-Flags, falls Display-Pin)
   - `main.cpp` (Initialisierung)

2. **Serial Monitor nutzen**: 
   ```cpp
   Serial.println("Debug-Info");
   ```
   Hilft beim Debugging!

3. **Kleine Schritte**: Teste eine Komponente nach der anderen

4. **Kommentare schreiben**: Embedded-Code kann komplex werden

