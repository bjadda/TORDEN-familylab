# Project Template

Copy this folder to `projects/NN-your-project-name/` to start a new project.

## Structure

```
NN-your-project-name/
  README.md          ← Fill this in first
  firmware/          ← PlatformIO project (ESP32 / Arduino)
    platformio.ini
    src/main.cpp
  hardware/          ← Schematics, BOM
  cad/               ← STL / STEP files
  ai/                ← Python scripts (if needed)
  docs/              ← Build guide, photos
```

## Checklist

- [ ] Fill in README.md — what does this project do?
- [ ] Add it to the table in the root README.md
- [ ] List your parts in hardware/BOM.md
- [ ] Add CAD files or a link to where they live
- [ ] Document how to build and flash it
- [ ] Use `docs/ai-inputs.md` for the first Claude/Codex planning pass
