// main.cpp — TORDEN-BOT entry point
// ESP32-CAM + PCA9685 servo driver + WebSocket remote control + MJPEG camera stream

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include "esp_camera.h"

// ─── WiFi ────────────────────────────────────────────────────────────────────
// TODO: Replace with your network credentials or use captive-portal on first boot
const char* WIFI_SSID = "YourNetwork";
const char* WIFI_PASS = "YourPassword";
const char* HOSTNAME  = "torden";

// ─── I2C pins (ESP32-CAM free pins) ──────────────────────────────────────────
#define I2C_SDA 14
#define I2C_SCL 15

// ─── Ultrasonic sensor ────────────────────────────────────────────────────────
#define TRIG_PIN 13
#define ECHO_PIN 12

// ─── Teach button ─────────────────────────────────────────────────────────────
#define TEACH_BTN 0

// ─── Camera config (AI Thinker ESP32-CAM) ────────────────────────────────────
#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK     0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27
#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      38
#define CAM_PIN_D3      37
#define CAM_PIN_D2      36
#define CAM_PIN_D1       5
#define CAM_PIN_D0       4
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22

WebServer       httpServer(80);
WebSocketsServer wsServer(81);

// ─── Forward declarations ─────────────────────────────────────────────────────
void initCamera();
void initWiFi();
void initServos();
void handleRoot();
void handleStream();
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);

// ─────────────────────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("[TORDEN] Booting…");

    Wire.begin(I2C_SDA, I2C_SCL);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(TEACH_BTN, INPUT_PULLUP);

    initServos();
    initCamera();
    initWiFi();

    httpServer.on("/",       handleRoot);
    httpServer.on("/stream", handleStream);
    httpServer.begin();

    wsServer.begin();
    wsServer.onEvent(onWebSocketEvent);

    Serial.printf("[TORDEN] Ready — http://%s.local\n", HOSTNAME);
}

void loop() {
    httpServer.handleClient();
    wsServer.loop();
}

// ─── WiFi ─────────────────────────────────────────────────────────────────────
void initWiFi() {
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("[TORDEN] Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print('.');
    }
    Serial.printf("\n[TORDEN] IP: %s\n", WiFi.localIP().toString().c_str());
}

// ─── Servos (PCA9685) ─────────────────────────────────────────────────────────
void initServos() {
    // TODO: initialise Adafruit_PWMServoDriver, set frequency to 50Hz
    // See: projects/01-torden-bot/firmware/src/servo/servo_ctrl.cpp
    Serial.println("[TORDEN] Servos ready (stub)");
}

// ─── Camera ───────────────────────────────────────────────────────────────────
void initCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer   = LEDC_TIMER_0;
    config.pin_d0  = CAM_PIN_D0;  config.pin_d1 = CAM_PIN_D1;
    config.pin_d2  = CAM_PIN_D2;  config.pin_d3 = CAM_PIN_D3;
    config.pin_d4  = CAM_PIN_D4;  config.pin_d5 = CAM_PIN_D5;
    config.pin_d6  = CAM_PIN_D6;  config.pin_d7 = CAM_PIN_D7;
    config.pin_xclk   = CAM_PIN_XCLK;
    config.pin_pclk   = CAM_PIN_PCLK;
    config.pin_vsync  = CAM_PIN_VSYNC;
    config.pin_href   = CAM_PIN_HREF;
    config.pin_sscb_sda = CAM_PIN_SIOD;
    config.pin_sscb_scl = CAM_PIN_SIOC;
    config.pin_pwdn   = CAM_PIN_PWDN;
    config.pin_reset  = CAM_PIN_RESET;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size   = FRAMESIZE_VGA;
    config.jpeg_quality = 12;
    config.fb_count     = psramFound() ? 2 : 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("[TORDEN] Camera init failed: 0x%x\n", err);
    } else {
        Serial.println("[TORDEN] Camera ready");
    }
}

// ─── HTTP handlers ────────────────────────────────────────────────────────────
void handleRoot() {
    // Serve minimal remote control UI — expand later
    httpServer.send(200, "text/html",
        "<html><head><title>TORDEN-BOT</title></head>"
        "<body><h1>TORDEN-BOT</h1>"
        "<img src='/stream' style='max-width:100%'>"
        "<p>WebSocket on port 81</p>"
        "</body></html>");
}

void handleStream() {
    WiFiClient client = httpServer.client();
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
    client.println();
    while (client.connected()) {
        camera_fb_t* fb = esp_camera_fb_get();
        if (!fb) continue;
        client.printf("--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", fb->len);
        client.write(fb->buf, fb->len);
        client.println();
        esp_camera_fb_return(fb);
    }
}

// ─── WebSocket events ─────────────────────────────────────────────────────────
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    if (type == WStype_TEXT) {
        StaticJsonDocument<128> doc;
        if (deserializeJson(doc, payload)) return;
        const char* cmd = doc["cmd"];
        if (!cmd) return;
        // TODO: route cmd → gait controller
        // e.g. "forward", "back", "left", "right", "stop", "teach_start", "teach_end"
        Serial.printf("[WS] cmd: %s\n", cmd);
    }
}
