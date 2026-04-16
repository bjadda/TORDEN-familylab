# ⚡ TORDEN — Family Maker Lab

> **TORDEN** (Norwegian: *thunder*) — a shared workspace for 3D printing, microelectronics, and AI projects built by a dad and his kids.

This repo is our open-source lab notebook. Every project lives here — from first sketches to finished builds. Everything is designed to be reproducible: full CAD files, firmware, wiring diagrams, BOM, and build guides included.

---

## 🤖 Active projects

| # | Project | Status | Tags |
|---|---------|--------|------|
| [01](projects/01-torden-bot/) | **TORDEN-BOT** — AI walking quadruped robot | 🔨 In progress | `ESP32` `3D-print` `servos` `AI` `vision` `teaching` |

---

## 💡 Ideas backlog

Not started yet — pick one and run with it. Roughly ordered by complexity.

### Beginner — first soldering / first print

| Idea | What it is | Key parts |
|------|-----------|-----------|
| **Weather station** | ESP32 reads temp/humidity/pressure, 3D-printed enclosure on the windowsill, syncs to a web dashboard | ESP32, BME280, e-ink display |
| **Name sign** | 3D-printed letters with WS2812B LEDs behind them — lights up, animations via phone | ESP32, WS2812B strip, custom letter molds |
| **Reaction game** | 3D-printed box with 4 coloured buttons and LEDs — plays Simon Says, records high scores | Arduino Nano, 4× LED, 4× button |
| **Plant watering bot** | Soil moisture sensor triggers a mini pump when it gets dry, logs readings | ESP32, capacitive moisture sensor, 5V pump, relay |
| **Fidget cube** | Fully mechanical — buttons, joystick, dials — nothing electronic, pure CAD and print | FDM print only |

### Intermediate — combining sensors + code + CAD

| Idea | What it is | Key parts |
|------|-----------|-----------|
| **Mechanical flip-clock** | 3D-printed split-flap display driven by servos, NTP time sync | ESP32, SG90 servos, PCA9685, NTP |
| **Mini arcade cabinet** | Tiny tabletop cabinet with screen + buttons, runs custom games | RP2040, 2.8" TFT, 3D-printed cabinet shell |
| **Gesture lamp** | Swipe hand over lamp to change colour/brightness — no buttons | ESP32, APDS9960 gesture sensor, NeoPixel ring, printed shade |
| **Star tracker** | Motorised mount slowly rotates a camera to follow stars — for long-exposure photos | ESP32, 2× stepper motor, A4988 driver, worm gear CAD |
| **Binary clock** | Tells time in binary using LEDs — learn binary while telling the time | Arduino, 12× LED matrix, DS3231 RTC module |

### Advanced — multi-week builds

| Idea | What it is | Key parts |
|------|-----------|-----------|
| **TORDEN-ARM** | 3D-printed 4-DOF robot arm — teach positions by hand, replay, controlled via web UI | ESP32, 4× MG996R, PCA9685, same teaching mode as TORDEN-BOT |
| **Autonomous rover** | Tracked chassis, maps a room using a TOF sensor array, avoids obstacles | ESP32, VL53L5CX TOF, L298N motor driver, tracked base |
| **AI thermal camera** | ESP32 + thermal array sensor shows hot spots in real time — useful for electronics debugging | ESP32, MLX90640, TFT display, 3D-printed gun housing |
| **Weather balloon payload** | GPS + sensors + camera in a printed enclosure, recovers after flight | ESP32, NEO-6M GPS, BME280, SD card logger |
| **Holographic persistence-of-vision display** | Spinning LED bar creates floating 3D image | ESP32, WS2812B strip, brushless motor + slip ring, CAD mount |

---

## 🛠️ Tools we use

| Category | Tool |
|----------|------|
| 3D modelling | Fusion 360 / FreeCAD |
| Slicing | PrusaSlicer / Bambu Studio |
| Firmware | PlatformIO (VS Code) + Arduino framework |
| AI / Python | TensorFlow Lite, OpenCV, Python 3 |
| Wiring diagrams | KiCad / Fritzing |
| Version control | Git + GitHub |

---

## 📁 Repo layout

```
projects/
  01-torden-bot/          # AI walking quadruped robot
  02-.../                 # Future projects
templates/
  project-template/       # Copy this to start a new project
```

---

## 🚀 Getting started

### Clone
```bash
git clone https://github.com/bjadda/TORDEN-ESP32.git
cd TORDEN-ESP32
```

### Flash firmware (PlatformIO)
```bash
cd projects/01-torden-bot/firmware
pio run --target upload
```

### Run AI scripts
```bash
cd projects/01-torden-bot/ai
pip install -r requirements.txt
python vision/detect.py
```

---

## 📜 License

MIT — see [LICENSE](LICENSE). Build it, mod it, share it.