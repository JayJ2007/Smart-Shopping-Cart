/*
 ESP32 Billing, RFID & Web Server Firmware
 Smart Shopping Cart Project

 Handles:
 - RFID-based user authentication
 - Wi-Fi connectivity
 - HTTP server for mobile app communication
 - Real-time billing display on LCD
*/

// ================= Libraries =================
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

// ================= Configuration =================
// ⚠️ Replace these with your own Wi-Fi credentials (do not commit real ones)
const char* ssid     = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Authorized RFID UID (example format: " 4F 41 CC C4")
String AUTHORIZED_UID = " 4F 41 CC C4";

// ================= Hardware Configuration =================
#define RST_PIN 5     // RFID Reset pin
#define SS_PIN  15    // RFID Slave Select (SDA)

MFRC522 mfrc522(SS_PIN, RST_PIN);

// I2C LCD (use address found via I2C scanner)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// HTTP server
WebServer server(80);

// ================= State Management =================
enum State {
  WAITING_FOR_ACCESS,
  ACCESS_GRANTED
};

State currentState = WAITING_FOR_ACCESS;

// ================= Function Declarations =================
void initializeNetwork();

// ================= HTTP Handlers =================
void handleUpdate() {
  String total = "0.00";

  if (server.hasArg("total")) {
    total = server.arg("total");
  }

  Serial.print("Received Total: ");
  Serial.println(total);

  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total Price:");
  lcd.setCursor(0, 1);
  lcd.print("Rs. " + total);

  server.send(200, "text/plain", "OK (Total Received)");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}

// ================= Setup =================
void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize RFID
  SPI.begin();
  mfrc522.PCD_Init();

  // Initial LCD message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Access Denied");
  lcd.setCursor(0, 1);
  lcd.print("  Please Scan");

  Serial.println("Waiting for authorized RFID card...");
}

// ================= Main Loop =================
void loop() {
  switch (currentState) {

    case WAITING_FOR_ACCESS:
      if (mfrc522.PICC_IsNewCardPresent() &&
          mfrc522.PICC_ReadCardSerial()) {

        String scannedUID = "";

        for (byte i = 0; i < mfrc522.uid.size; i++) {
          scannedUID += (mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          scannedUID += String(mfrc522.uid.uidByte[i], HEX);
        }

        scannedUID.toUpperCase();

        if (scannedUID.equals(AUTHORIZED_UID)) {
          // ----- ACCESS GRANTED -----
          Serial.println("Access Granted!");

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Access Granted ");
          delay(2000);

          initializeNetwork();
          currentState = ACCESS_GRANTED;

        } else {
          // ----- ACCESS DENIED -----
          Serial.println("Access Denied!");

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Access Denied!");
          delay(2000);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" Access Denied");
          lcd.setCursor(0, 1);
          lcd.print("  Please Scan");
        }

        // Cleanup RFID state
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
      }
      break;

    case ACCESS_GRANTED:
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi lost. Reconnecting...");
        initializeNetwork();
      }

      server.handleClient();
      break;
  }
}

// ================= Network Initialization =================
void initializeNetwork() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP Address:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // HTTP endpoints
  server.on("/update", HTTP_POST, handleUpdate);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SmartCart Ready");
  lcd.setCursor(0, 1);
  lcd.print("Total: Rs. 0.00");
}
