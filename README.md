# ⚡ TORDEN — Family Maker Lab

> **TORDEN** (Norwegian: *thunder*) — a shared workspace for 3D printing, microelectronics, and AI projects built by a dad and his kids.

This repo is our open-source lab notebook. Every project lives here — from first sketches to finished builds. Everything is designed to be reproducible: full CAD files, firmware, wiring diagrams, BOM, and build guides included.

---

## 🤖 Projects

| # | Project | Status | Tags |
|---|---------|--------|------|
| [01](projects/01-torden-bot/) | **TORDEN-BOT** — AI walking quadruped robot | 🔨 In progress | `ESP32` `3D-print` `servos` `AI` `vision` `teaching` |

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