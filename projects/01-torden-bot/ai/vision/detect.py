#!/usr/bin/env python3
"""
detect.py — Real-time object detection using TFLite MobileNet-SSD
Reads frames from Pi Camera, runs inference, sends bounding box data
to ESP32 over UART serial.

Usage:
    python3 detect.py [--serial /dev/serial0] [--threshold 0.5]
"""

import argparse
import json
import re
import serial
import time
from pathlib import Path

import cv2
import numpy as np

try:
    import tflite_runtime.interpreter as tflite
except ImportError:
    import tensorflow.lite as tflite

MODEL_PATH  = Path(__file__).parent / "models" / "detect.tflite"
LABELS_PATH = Path(__file__).parent / "models" / "labelmap.txt"

def load_labels(path: Path) -> list[str]:
    lines = path.read_text().splitlines()
    # Strip index prefix if present (e.g. "0 person")
    return [re.sub(r'^\d+\s+', '', l) for l in lines if l.strip()]

def run(serial_port: str, threshold: float):
    labels      = load_labels(LABELS_PATH)
    interpreter = tflite.Interpreter(model_path=str(MODEL_PATH))
    interpreter.allocate_tensors()

    in_details  = interpreter.get_input_details()
    out_details = interpreter.get_output_details()
    height, width = in_details[0]['shape'][1:3]

    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        raise RuntimeError("Cannot open camera")

    ser = None
    if serial_port:
        ser = serial.Serial(serial_port, 115200, timeout=0)
        print(f"[detect] Serial: {serial_port}")

    print(f"[detect] Running — model={MODEL_PATH.name} threshold={threshold}")

    while True:
        ret, frame = cap.read()
        if not ret:
            break

        rgb   = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        resized = cv2.resize(rgb, (width, height))
        input_data = np.expand_dims(resized, axis=0)

        interpreter.set_tensor(in_details[0]['index'], input_data)
        interpreter.invoke()

        boxes   = interpreter.get_tensor(out_details[0]['index'])[0]
        classes = interpreter.get_tensor(out_details[1]['index'])[0]
        scores  = interpreter.get_tensor(out_details[2]['index'])[0]

        detections = []
        for i, score in enumerate(scores):
            if score < threshold:
                continue
            ymin, xmin, ymax, xmax = boxes[i]
            label = labels[int(classes[i])] if int(classes[i]) < len(labels) else "?"
            detections.append({
                "label": label,
                "score": round(float(score), 2),
                "box":   [round(float(v), 3) for v in [xmin, ymin, xmax, ymax]],
            })

        if detections and ser:
            msg = json.dumps({"detections": detections}) + "\n"
            ser.write(msg.encode())

        # Optional local preview (disable on headless Pi)
        # cv2.imshow("TORDEN detect", frame)
        # if cv2.waitKey(1) & 0xFF == ord('q'):
        #     break

    cap.release()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--serial",    default="/dev/serial0",
                        help="UART port to ESP32 (or empty to disable)")
    parser.add_argument("--threshold", type=float, default=0.5,
                        help="Detection confidence threshold (0–1)")
    args = parser.parse_args()
    run(args.serial, args.threshold)
