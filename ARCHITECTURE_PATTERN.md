# Architecture Pattern - MVC für Chess Clock

## 🤔 Ist MVC hier sinnvoll?

### ✅ **JA, aber mit Anpassungen für Embedded-Systeme**

MVC ist grundsätzlich eine gute Idee, aber für Embedded-Systeme gibt es bessere Alternativen:

---

## 📊 Vergleich: MVC vs. Embedded-optimierte Patterns

### **Klassisches MVC**
```
Model      → Daten & Business-Logik (State Machine, Timer)
View       → Darstellung (Display-Rendering)
Controller → Steuerung (Input-Handling, State-Transitions)
```

**Probleme für Embedded:**
- Controller wird oft zu komplex
- View und Controller sind oft zu eng gekoppelt
- Memory-Overhead durch zusätzliche Abstraktionsebenen

### **Besser: MVP (Model-View-Presenter) oder einfache Trennung**
```
Model      → State Machine, Timer, Game Logic
View       → Display-Rendering (nur Darstellung)
Presenter  → Verbindet Model und View (einfacher als Controller)
```

**Vorteile:**
- Klarere Trennung
- Weniger Overhead
- Einfacher zu testen
- Besser für Embedded-Systeme

---

## 🎯 Empfohlene Architektur für Chess Clock

### **Pattern: Model-View-Presenter (MVP) - Lightweight**

```
┌─────────────────────────────────────────┐
│           main.cpp (Orchestrator)       │
└─────────────────────────────────────────┘
                    │
        ┌───────────┼───────────┐
        │           │           │
        ▼           ▼           ▼
┌───────────┐ ┌───────────┐ ┌───────────┐
│   Model   │ │  Presenter │ │   View    │
│           │ │            │ │           │
│ State     │ │ State      │ │ Display   │
│ Machine   │ │ Transitions │ │ Rendering │
│ Timer     │ │ Input       │ │ UI        │
│ Game Data │ │ Handling    │ │           │
└───────────┘ └───────────┘ └───────────┘
```

### **Komponenten:**

#### **1. Model (Daten & Logik)**
- State Machine (Zustände)
- Timer-Logik (Zeitverwaltung)
- Spiel-Daten (Spieler, Zeiten)
- Game Rules (Regeln)

#### **2. View (Darstellung)**
- Display-Rendering
- UI-Elemente zeichnen
- Keine Business-Logik!

#### **3. Presenter (Vermittler)**
- Input-Handling (Button, Encoder)
- State-Transitions
- Verbindet Model ↔ View
- Event-Handling

---

## 📁 Empfohlene Dateistruktur

```
firmware/
├── src/
│   ├── main.cpp              # Orchestrator (setup/loop)
│   │
│   ├── model/                 # Model Layer
│   │   ├── state_machine.cpp  # State Machine Logic
│   │   ├── timer.cpp          # Timer Management
│   │   ├── game_data.cpp      # Game State Data
│   │   └── game_rules.cpp     # Game Rules
│   │
│   ├── view/                  # View Layer
│   │   ├── display.cpp        # Display Rendering
│   │   ├── ui_idle.cpp        # UI für IDLE State
│   │   ├── ui_menu.cpp        # UI für Menu States
│   │   ├── ui_game.cpp        # UI für Game States
│   │   └── ui_common.cpp      # Gemeinsame UI-Funktionen
│   │
│   └── presenter/             # Presenter Layer
│       ├── input_handler.cpp  # Button/Encoder Input
│       ├── state_controller.cpp # State Transitions
│       └── event_dispatcher.cpp # Event Handling
│
├── include/
│   ├── model/
│   │   ├── state_machine.h
│   │   ├── timer.h
│   │   ├── game_data.h
│   │   └── game_rules.h
│   │
│   ├── view/
│   │   ├── display.h
│   │   ├── ui_idle.h
│   │   ├── ui_menu.h
│   │   ├── ui_game.h
│   │   └── ui_common.h
│   │
│   └── presenter/
│       ├── input_handler.h
│       ├── state_controller.h
│       └── event_dispatcher.h
│
└── platformio.ini
```

---

## 💡 Warum MVP statt MVC?

### **1. Einfacher für Embedded**
```cpp
// MVC - Controller muss alles wissen
class Controller {
  void handleInput() {
    // Muss Model UND View kennen
    model->update();
    view->render(model->getState());
  }
};

// MVP - Presenter ist einfacher
class Presenter {
  void handleInput() {
    // Nur Model updaten, View wird automatisch aktualisiert
    model->update();
    updateView(); // Einfacher
  }
};
```

### **2. Bessere Testbarkeit**
- Model kann ohne Hardware getestet werden
- View kann mit Mock-Daten getestet werden
- Presenter verbindet beides

### **3. Klarere Verantwortlichkeiten**
- **Model**: "Was ist der Zustand?"
- **View**: "Wie wird es dargestellt?"
- **Presenter**: "Wie reagieren wir auf Input?"

---

## 🔄 Datenfluss

### **Typischer Ablauf:**

```
1. User drückt Button
   ↓
2. Input Handler (Presenter) erkennt Input
   ↓
3. Presenter ruft State Machine (Model) auf
   ↓
4. State Machine ändert State
   ↓
5. Presenter benachrichtigt View
   ↓
6. View rendert neuen State
```

### **Code-Beispiel:**

```cpp
// main.cpp
void loop() {
  // 1. Input lesen
  presenter.handleInput();
  
  // 2. State Machine aktualisieren
  presenter.updateStateMachine();
  
  // 3. View aktualisieren
  presenter.updateView();
  
  delay(10);
}

// presenter/state_controller.cpp
void StateController::handleButtonPress() {
  ChessClockState currentState = model->getCurrentState();
  
  // State Transition basierend auf aktuellem State
  switch(currentState) {
    case ChessClockState::IDLE:
      model->transitionTo(ChessClockState::MAIN_MENU);
      view->showMainMenu();
      break;
    // ...
  }
}

// view/ui_menu.cpp
void UIMenu::showMainMenu() {
  display->clear();
  display->drawText("Main Menu", ...);
  display->drawMenuItem("Play Game", ...);
  // Nur Rendering, keine Logik!
}
```

---

## ⚠️ Wichtige Prinzipien

### **1. Model kennt View NICHT**
```cpp
// ❌ SCHLECHT
class StateMachine {
  void transition() {
    view->update(); // Model sollte View nicht kennen!
  }
};

// ✅ GUT
class StateMachine {
  void transition() {
    // Nur State ändern
    currentState = newState;
  }
};
```

### **2. View kennt Model NICHT direkt**
```cpp
// ❌ SCHLECHT
void UIMenu::render() {
  if (model->getState() == ...) { // View sollte Model nicht kennen!
    // ...
  }
}

// ✅ GUT
void UIMenu::render(const GameData& data) {
  // Bekommt nur Daten, keine Logik
  display->drawText(data.player1Name, ...);
}
```

### **3. Presenter koordiniert alles**
```cpp
// ✅ GUT
class Presenter {
  void updateView() {
    ChessClockState state = model->getCurrentState();
    GameData data = model->getGameData();
    
    switch(state) {
      case ChessClockState::IDLE:
        view->renderIdle(data);
        break;
      case ChessClockState::MAIN_MENU:
        view->renderMainMenu(data);
        break;
      // ...
    }
  }
};
```

---

## 🎯 Vorteile für dein Projekt

### **1. Wartbarkeit**
- State Machine kann geändert werden ohne View zu ändern
- UI kann geändert werden ohne State Machine zu ändern

### **2. Testbarkeit**
- Model kann unit-getestet werden
- View kann mit Mock-Daten getestet werden

### **3. Erweiterbarkeit**
- Neue States: Nur Model + View erweitern
- Neue UI-Elemente: Nur View erweitern
- Neue Inputs: Nur Presenter erweitern

### **4. Debugging**
- Klare Trennung macht Fehler leichter zu finden
- Jede Schicht kann einzeln getestet werden

---

## 📝 Implementierungs-Plan

### **Phase 1: Grundstruktur**
1. Model erstellen (State Machine, Timer)
2. View erstellen (Display-Rendering)
3. Presenter erstellen (Input-Handling)

### **Phase 2: Integration**
1. State Transitions implementieren
2. UI für jeden State erstellen
3. Input-Handling verbinden

### **Phase 3: Optimierung**
1. Performance optimieren
2. Memory-Usage optimieren
3. Code refactoren

---

## 🔄 Alternative: Einfache Trennung (wenn MVP zu komplex)

Falls MVP zu viel Overhead ist, reicht auch eine einfache Trennung:

```
firmware/
├── src/
│   ├── main.cpp
│   ├── state_machine.cpp    # Logik
│   ├── display.cpp          # Darstellung
│   └── input.cpp            # Input
```

**Regel**: 
- State Machine kennt Display nicht
- Display bekommt Daten von State Machine
- Input triggert State Machine

---

## ✅ Empfehlung

**Für dein Chess Clock Projekt:**

1. **Starte mit einfacher Trennung** (State Machine, Display, Input)
2. **Wenn es komplexer wird** → MVP Pattern
3. **Vermeide vollständiges MVC** (zu komplex für Embedded)

**Warum?**
- Embedded-Systeme haben begrenzte Ressourcen
- Einfache Trennung ist oft ausreichend
- MVP ist ein guter Kompromiss zwischen Struktur und Overhead

---

## 📚 Weiterführende Ressourcen

- **MVP Pattern**: https://en.wikipedia.org/wiki/Model–view–presenter
- **Embedded Software Architecture**: Best Practices für Embedded-Systeme
- **State Machine Patterns**: Für komplexe State Machines

