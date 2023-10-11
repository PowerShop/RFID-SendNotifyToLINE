// Library for send LINE Notify
#include <TridentTD_LineNotify.h>

// Library for LCD 16x2 (1602A)
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Library for RFID
#include <SPI.h>
#include <MFRC522.h>

// Define pin
#define RST_PIN D3
#define SS_PIN D4

// Define value for LINE Notify
#define SSID "MM_2G"
#define PASSWORD "08484840"
#define LINE_TOKEN "J2RgXWAGi921SG6wUMmpVaeXTnS0zDwJTDRwQ1AFciD"

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

String rfid_in = "";

void setup() {

  // Setup for Serial
  Serial.begin(9600);

  // Setup for RFID
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("");

  // Setup for LCD
  lcd.init();
  lcd.backlight();

  // Setup for LINE Notify
  Serial.println(LINE.getVersion());

  // เชื่อมต่อ WIFI
  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n", SSID);
  lcd.setCursor(0, 0);
  lcd.print("Connecting to...");
  lcd.setCursor(0, 1);
  lcd.print(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }

  // เชื่อมต่อ WIFI สำเร็จ
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  clearLCDLine(0);
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected");
  delay(3000);
  clearLCDLine(0);
  clearLCDLine(1);

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Ready to scan!");
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    lcd.clear();
    rfid_in = rfid_read();
    // lcd.clear();
    Serial.println(">>>> " + rfid_in);
    lcd.setCursor(0, 0);
    lcd.print(rfid_in);
    lcd.setCursor(0, 1);

    lcd.print("Wait... 3");
    delay(1000);
    clearLCDLine(1);
    lcd.print("Wait... 2");
    delay(1000);
    clearLCDLine(1);
    lcd.print("Wait... 1");
    delay(1000);
    clearLCDLine(1);
    lcd.print("OK! next...");
    delay(1000);


    // delay(3000);
    lcd.clear();
    delay(500);
  }
  // Write my code at here:
  if (rfid_in == "17 9A DA 29") {
    LINE.notify("แจ้งเตือน: กิตติศักดิ์ได้ทำการแตะบัตร");
    // lcd.setCursor(0, 0);
    // lcd.print("LCD1602 I2c Test");
    delay(100);
    rfid_in = "";
  }

  delay(1);
}

String rfid_read() {
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  return content.substring(1);
}

// ลบข้อความ
void clearLCDLine(int line) {
  lcd.setCursor(0, line);
  for (int n = 0; n < 16; n++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, line);
}