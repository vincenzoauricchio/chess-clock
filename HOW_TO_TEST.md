# Wie man die Serial Input Simulation testet

## ⚠️ Wichtig: Serial Monitor blockiert den Port!

Wenn der Serial Monitor geöffnet ist, kann kein Code hochgeladen werden.

## 📋 Schritt-für-Schritt Anleitung

### 1. Serial Monitor schließen (falls geöffnet)
Im Terminal wo der Serial Monitor läuft:
```
Ctrl+C  (Serial Monitor beenden)
```

### 2. Code kompilieren und hochladen
```bash
cd firmware
pio run -t upload
```

**Oder in VS Code/Cursor:**
- PlatformIO Symbolleiste → Upload Button (→)

### 3. Serial Monitor öffnen
```bash
pio device monitor
```

**Oder in VS Code/Cursor:**
- PlatformIO Symbolleiste → Serial Monitor Button (🔌)

### 4. Was du sehen solltest

Nach dem Öffnen des Serial Monitors sollte erscheinen:

```
[INFO]: Chess Clock
[INFO]: Display initialized
[INFO]: State Controller initialized
[INFO]: System ready!

=== Chess Clock - Input Simulation ===
BUTTONS:
  p, press, rotary     - Press rotary encoder button
  1, b1, w, white      - Press Button 1 (White/Player 1)
  2, b2, bl, black     - Press Button 2 (Black/Player 2)
...
```

### 5. Befehle testen

Im Eingabefeld des Serial Monitors:

```
> s                    ← Aktuellen State anzeigen
> p                    ← Rotary Button drücken
> 1                    ← Button 1 (Weiß) drücken
> 2                    ← Button 2 (Schwarz) drücken
> r                    ← Rotary rechts drehen
> l                    ← Rotary links drehen
> h                    ← Hilfe anzeigen
```

## 🔧 Troubleshooting

### Problem: "Port is busy"
**Lösung:** Serial Monitor schließen (`Ctrl+C`), dann Upload, dann Serial Monitor wieder öffnen

### Problem: Keine Ausgabe im Serial Monitor
**Mögliche Ursachen:**
1. Code wurde nicht hochgeladen → `pio run -t upload` ausführen
2. Falscher Port → Port in PlatformIO prüfen
3. ESP32 nicht verbunden → USB-Kabel prüfen
4. Baudrate falsch → Sollte 115200 sein (automatisch in platformio.ini)

### Problem: ESP32 startet nicht
**Lösung:** 
- Reset-Button am ESP32 drücken
- Oder USB-Kabel kurz ab- und wieder anschließen

## 💡 Tipp

**Workflow für schnelles Testen:**
1. Code ändern
2. Serial Monitor schließen (`Ctrl+C`)
3. Upload (`pio run -t upload`)
4. Serial Monitor öffnen (`pio device monitor`)
5. Testen

## 📝 Quick Reference

| Befehl | Aktion |
|--------|--------|
| `p` | Rotary Button drücken |
| `1` | Button 1 (Weiß) drücken |
| `2` | Button 2 (Schwarz) drücken |
| `r` | Rotary rechts drehen |
| `l` | Rotary links drehen |
| `s` | Aktuellen State anzeigen |
| `h` | Hilfe anzeigen |

