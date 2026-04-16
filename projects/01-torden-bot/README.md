# 🤖 TORDEN-BOT — AI Walking Quadruped Robot

An open-source, fully 3D-printed quadruped robot with a live camera feed, object detection, autonomous walking, and a **teaching mode** where kids can record new movement sequences just by guiding the robot's legs.

Built around an ESP32-CAM as the main controller and an optional Raspberry Pi as an AI co-processor.

> **Current state:** Architecture, BOM, and firmware scaffold are done. CAD design and full firmware implementation are the next big milestones — see [Project status](#project-status). The design is validated against real community builds (SpotMicro, OpenQuadruped) — it can absolutely be built, but there is real work ahead.

---

## Features

| Feature | How |
|---------|-----|
| **Walking gaits** | Trot, creep, and turn — inverse kinematics on ESP32 |
| **Live video** | ESP32-CAM streams MJPEG to any browser on the local network |
| **Object detection** | TFLite MobileNet-SSD on Pi; bounding boxes overlaid on stream |
| **Obstacle avoidance** | HC-SR04 ultrasonic sensor; halts and reroutes autonomously |
| **Teaching mode** | Record a leg-movement sequence by hand, save to flash, replay on command |
| **Remote control** | WebSocket-based gamepad UI — works on phone or tablet |
| **Voice commands** | Wake word ("Hey TORDEN") + action keywords via microphone on Pi |
| **Face tracking** | Pan/tilt head follows a detected face |

---

## Hardware

### Bill of Materials

| Part | Qty | Approx. cost |
|------|-----|-------------|
| ESP32-CAM (AI Thinker) | 1 | €8 |
| Raspberry Pi Zero 2W *(AI co-processor, optional)* | 1 | €18 |
| PCA9685 16-channel PWM servo driver (I2C) | 1 | €5 |
| MG996R digital servo (legs) | 8 | €18 |
| SG90 micro servo (head pan/tilt) | 2 | €4 |
| HC-SR04 ultrasonic distance sensor | 1 | €2 |
| INMP441 I2S microphone *(voice, optional)* | 1 | €4 |
| 7.4V 2S LiPo battery 2200mAh | 1 | €14 |
| 2S LiPo BMS / protection board | 1 | €4 |
| Mini DC-DC buck converter (7.4V → 5V **5A**) | 1 | €5 |
| PLA filament for 3D printed parts | ~200g | €5 |
| M2 / M3 screws and heat-set inserts | assorted | €4 |
| Dupont wires, JST connectors | assorted | €3 |
| **Total** | | **~€92** |

### Wiring overview

```
LiPo 7.4V
  ├─► BMS ─► 7.4V rail ─► ESP32-CAM (via AMS1117 3.3V on board)
  └─► Buck 5V ─► PCA9685 V+ ─► 10× servos
                └─► Pi Zero 2W (via USB-C 5V)

ESP32-CAM
  ├─ I2C (GPIO14=SDA, GPIO15=SCL) ─► PCA9685
  ├─ GPIO13 ─► HC-SR04 TRIG
  ├─ GPIO12 ─► HC-SR04 ECHO
  └─ UART TX/RX ─► Pi Zero 2W (serial bridge for AI results)

Pi Zero 2W
  ├─ CSI ─► Pi Camera Module 3 (or USB webcam)
  ├─ I2S ─► INMP441 mic
  └─ USB ─► (power only from buck converter)
```

### ⚠️ ESP32-CAM GPIO constraints

The AI Thinker ESP32-CAM has very few free GPIO pins when the camera is active. The SD card interface shares GPIO pins — if you use an SD card, I2C must move.

| Pin | Default use | TORDEN-BOT use | Conflict? |
|-----|------------|----------------|-----------|
| GPIO0 | Boot / flash | TEACH button | Use briefly; hold LOW only to flash |
| GPIO4 | Flash LED | Camera D0 | Camera takes priority; LED disabled |
| GPIO12 | SD DAT2 | HC-SR04 ECHO | Safe if **no SD card** fitted |
| GPIO13 | SD DAT3 | HC-SR04 TRIG | Safe if **no SD card** fitted |
| GPIO14 | SD CLK | I2C SDA | Safe if **no SD card** fitted |
| GPIO15 | SD CMD | I2C SCL | Safe if **no SD card** fitted |

**Recommendation:** Leave the SD card slot empty. Use NVS flash for teaching mode storage (already planned).

### ⚠️ Power budget

8× MG996R servos can draw up to 500mA stall each → worst case ~4A. Use a **5A** buck converter, not 3A. The LiPo should be ≥2200mAh 30C rated.



---

## 3D Printed Parts

> **Status:** Part names and print settings are designed; STL files are not created yet. CAD work in Fusion 360 is the first real build milestone.

All parts designed to print without supports on a standard FDM printer.
Print in **PLA** or **PETG** (PETG recommended for leg joints).

| File | Description | Print settings |
|------|-------------|---------------|
| `cad/body/body_main.stl` | Main chassis — houses ESP32, PCA9685, battery | 20% infill, 3 walls |
| `cad/body/battery_tray.stl` | LiPo tray with retention clip | 30% infill |
| `cad/legs/coxa.stl` | Hip segment × 4 | 40% infill |
| `cad/legs/femur.stl` | Upper leg × 4 | 40% infill |
| `cad/legs/tibia.stl` | Lower leg × 4 | 40% infill |
| `cad/legs/foot_pad.stl` | Rubber-tipped foot × 4 | TPU 95A |
| `cad/head/head_shell.stl` | Camera head housing | 20% infill |
| `cad/head/pan_bracket.stl` | Pan servo mount | 40% infill |
| `cad/head/tilt_bracket.stl` | Tilt servo mount | 40% infill |

Layer height: 0.2mm · Nozzle: 0.4mm

---

## Firmware

> **Status:** `main.cpp` scaffold exists (WiFi, MJPEG stream, WebSocket server, servo stub). The modules below are planned — structure is defined, code is not written yet.

Built with **PlatformIO** (Arduino framework).

```
firmware/
  platformio.ini         # Board: esp32cam, framework: arduino
  src/
    main.cpp             # Entry point — WiFi, WebSocket server, servo init
    gait/
      gait.h / .cpp      # Inverse kinematics + gait patterns (trot, creep, turn)
    servo/
      servo_ctrl.h/.cpp  # PCA9685 driver over I2C
    camera/
      camera_stream.h    # MJPEG streaming handler
    sensors/
      ultrasonic.h/.cpp  # HC-SR04 non-blocking distance reading
    teaching/
      teaching.h/.cpp    # Record / store / replay movement sequences in NVS
    comms/
      ws_server.h/.cpp   # WebSocket server (remote control + AI commands from Pi)
```

### Flash

```bash
cd projects/01-torden-bot/firmware
pio run --target upload --upload-port /dev/ttyUSB0
```

### Configure WiFi

Edit `firmware/src/main.cpp`:
```cpp
const char* WIFI_SSID = "YourNetwork";
const char* WIFI_PASS = "YourPassword";
```

Or use the captive portal on first boot (AP mode `TORDEN-SETUP`).

---

## AI (Raspberry Pi Zero 2W)

```
ai/
  requirements.txt       # opencv-python, tflite-runtime, pyaudio, vosk
  vision/
    detect.py            # MobileNet-SSD object detection → serial to ESP32
    face_track.py        # Face detection + PID pan/tilt control
    models/              # .tflite model files (downloaded separately — see below)
  teaching/
    record_teach.py      # Capture & label teaching sequences from serial
  voice/
    wake_word.py         # Vosk offline wake word + command recognition
```

### Setup on Pi Zero 2W

```bash
sudo apt update && sudo apt install python3-pip libatlas-base-dev -y
pip3 install -r requirements.txt

# Download TFLite model (~4MB)
wget https://storage.googleapis.com/download.tensorflow.org/models/tflite/coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.zip
unzip -j coco_ssd_mobilenet_v1_1.0_quant_2018_06_29.zip -d ai/vision/models/

# Run object detection
python3 ai/vision/detect.py
```

---

## Teaching Mode

Teaching mode lets kids physically guide the robot's legs and save the movement as a named action.

1. Hold the **TEACH** button (GPIO0 on ESP32) for 2 seconds — LEDs pulse blue
2. Slowly move the legs to each position in the sequence
3. Press **RECORD** to save each keyframe
4. Press **TEACH** again to end recording — sequence saved to flash with auto-name (`move_001`, `move_002`, …)
5. Replay any saved move via the web UI or say *"Hey TORDEN, do move one"*

Saved sequences are stored in ESP32 NVS (non-volatile storage) and survive power cycles.

---

## Web UI (remote control)

Once connected to WiFi, open `http://torden.local` (or the IP shown on serial).

- **D-pad** — walk in any direction
- **Camera** — live MJPEG stream
- **Speed slider** — gait frequency
- **Teach** — record/replay panel
- **Detection overlay** — bounding boxes from Pi AI
- **Settings** — WiFi, servo trim, gait tuning

---

## Build phases

| Phase | What we build | Skills learned |
|-------|--------------|---------------|
| **1 — Print & assemble** | Print all parts, install heat-set inserts, mount servos | 3D printing, mechanical assembly |
| **2 — Electronics** | Wire ESP32 + PCA9685 + power system, test each servo | Electronics, soldering, PWM |
| **3 — Walking** | Flash firmware, tune IK parameters, get it walking | C++, inverse kinematics, debugging |
| **4 — Camera & stream** | Enable MJPEG stream, build web UI | Networking, HTML/JS |
| **5 — AI vision** | Set up Pi, run object detection, face tracking | Python, machine learning |
| **6 — Teaching mode** | Record & replay movements | Data structures, UX design |
| **7 — Voice** | Wake word + commands | Speech recognition, audio processing |

---

## Project status

- [ ] CAD — body chassis v1
- [ ] CAD — leg assembly (coxa / femur / tibia)
- [ ] CAD — camera head
- [ ] Firmware — servo driver + IK
- [ ] Firmware — trot gait
- [ ] Firmware — camera stream
- [ ] Firmware — WebSocket remote control
- [ ] Firmware — teaching mode
- [ ] AI — object detection pipeline
- [ ] AI — face tracking
- [ ] AI — voice commands
- [ ] Web UI — gamepad + stream
- [ ] Docs — assembly guide with photos
- [ ] Docs — wiring diagram (KiCad)

---

## Useful references

- [ESP32-CAM datasheet](https://loboris.eu/ESP32/ESP32-CAM%20Product%20Specification.pdf)
- [PCA9685 datasheet](https://www.nxp.com/docs/en/data-sheet/PCA9685.pdf)
- [TensorFlow Lite for microcontrollers](https://www.tensorflow.org/lite/microcontrollers)
- [Vosk offline speech recognition](https://alphacephei.com/vosk/)
- [Spot Micro (inspiration)](https://github.com/mike4192/spotMicro)
- [OpenQuadruped (inspiration)](https://github.com/adham-elarabawy/open-quadruped)

---

*Built with ⚡ by bjadda and kids — TORDEN Family Maker Lab*
