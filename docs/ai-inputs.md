# TORDEN AI Inputs

Use this as the first message when asking Claude or Codex to help with a new TORDEN hardware project.

---

## Baseline Context

You are working in the `TORDEN-familylab` repo, a family maker lab for kid-friendly ESP32, 3D-printing, robotics, AI, and home-dashboard projects.

Repo conventions:

- Each project lives under `projects/NN-project-name/`.
- Use the existing `templates/project-template/README.md` structure.
- Firmware should default to PlatformIO + Arduino framework unless the project clearly fits ESPHome.
- Document wiring, BOM, flashing, safety limits, and test steps.
- Prefer reproducible, local-first designs over cloud-only behavior.
- Assume parts are inexpensive and available from typical maker suppliers or AliExpress.
- Keep the first milestone small enough for a weekend prototype.

Output format:

1. Project summary
2. Minimum viable build
3. BOM with approximate quantities
4. Wiring table
5. Firmware architecture
6. CAD/enclosure notes
7. Safety notes
8. Test plan
9. Files to create or edit in this repo

---

## M5Stack/CoreS3 Companion Prompt

Build a TORDEN project plan for an M5Stack CoreS3 or M5Stick-style desktop companion for Claude/Codex workflows.

Goals:

- Show current agent/session state on the device display.
- Provide one or two physical buttons that act as BLE HID shortcuts.
- Accept small JSON status payloads over BLE or local HTTP.
- Show useful states: idle, working, waiting for input, error, usage high, done.
- Include a kid-friendly animation mode that can run without exposing account data.

Reference patterns to study:

- StackChan for small expressive robot/device behavior.
- Clawdmeter for ESP32-S3 AMOLED, BLE daemon, usage/state display, and board abstraction.
- ESP Web Tools for browser-based flashing.

Constraints:

- Do not store API keys on the ESP32.
- If usage data is needed, read it from a host daemon on the computer.
- Keep firmware modular: `display`, `transport`, `input`, `state`, `ui`.
- Include a no-cloud demo mode using mocked JSON.

---

## E-Paper Family Dashboard Prompt

Build a TORDEN project plan for a Waveshare e-paper family dashboard that mirrors selected Aurora data.

Goals:

- Show today’s chores, weather, dinner plan, shopping priorities, and a short family note.
- Refresh on a schedule and on button press.
- Work in read-only mode first.
- Support an offline fallback screen showing the last successful sync time.

Likely hardware:

- Waveshare 7.5" or 10.85" e-paper
- ESP32 with enough RAM, or Raspberry Pi Zero 2W for easier rendering
- Optional battery/USB-C power

Constraints:

- Avoid personal data in logs.
- Render text large enough to read from a kitchen counter.
- Keep the first version as a single endpoint fetch plus static layout.

---

## Mini SCADA Trainer Prompt

Build a TORDEN project plan for a kid-safe tabletop SCADA trainer.

Goals:

- Teach sensors, actuators, alarms, dashboards, and safe control loops.
- Start with LEDs standing in for pumps/valves before moving to real water.
- Provide a browser dashboard with status lamps, alarm history, and manual override.

Likely hardware:

- ESP32
- LEDs or 12V/24V low-power pump with MOSFET/relay
- Float switch, water-level sensor, or simulated input switches
- Buzzer, status LEDs, emergency stop button

Safety constraints:

- First milestone must be dry/simulated.
- Any pump build needs low voltage, fuse, drip protection, and adult supervision.

---

## Claude/Codex Review Prompt

After generating a plan or code, ask:

```text
Review this TORDEN project for buildability. Focus on pin conflicts, power budget, missing safety limits, missing setup steps, unrealistic assumptions for kids, and files that do not match the repo structure. Return only actionable findings with file paths or section names.
```

