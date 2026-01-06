#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_MAX31865.h>
#include <Adafruit_INA219.h>
#include "logo.h" 

// --- PIN TANIMLARI ---
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   4
#define MAX1_CS   5
#define MAX2_CS   26
#define BATARYA_PIN 34 

// --- AYARLAR ---
#define BATARYA_MAX_V 4.20
#define BATARYA_MIN_V 3.20
#define VOLTAJ_CARPANI  0.00170 // Kalibrasyonunu unutma
#define AKIM_DUZELTME 1.02
#define LOGO_GENISLIK 320
#define LOGO_YUKSEKLIK 240
#define RREF      430.0
#define RNOMINAL  100.0

// --- NESNELER ---
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
Adafruit_MAX31865 sensor1 = Adafruit_MAX31865(MAX1_CS);
Adafruit_MAX31865 sensor2 = Adafruit_MAX31865(MAX2_CS);
Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  pinMode(BATARYA_PIN, INPUT); 

  sensor1.begin(MAX31865_3WIRE);
  sensor2.begin(MAX31865_3WIRE);
  if (!ina219.begin()) { Serial.println("INA219 Hatasi!"); }

  tft.begin();
  tft.setRotation(1);

  // Açılış Logosu
  tft.drawRGBBitmap(0, 0, logo_resmi, LOGO_GENISLIK, LOGO_YUKSEKLIK);
  delay(3000); 

  arayuzuCiz(); 
}

void loop() {
  // --- VERİ OKUMA ---
  float temp1 = sensor1.temperature(RNOMINAL, RREF);
  float temp2 = sensor2.temperature(RNOMINAL, RREF);
  sensor1.clearFault(); sensor2.clearFault();

  float current_mA = ina219.getCurrent_mA() * AKIM_DUZELTME;

  // Batarya Okuma
  long toplamADC = 0;
  for(int i=0; i<20; i++){ toplamADC += analogRead(BATARYA_PIN); delay(2); }
  float bataryaVoltaji = (toplamADC / 20.0) * VOLTAJ_CARPANI; 
  int pilYuzdesi = map(bataryaVoltaji * 100, BATARYA_MIN_V * 100, BATARYA_MAX_V * 100, 0, 100);
  pilYuzdesi = constrain(pilYuzdesi, 0, 100);

  // --- EKRAN GÜNCELLEME ---

  // 1. Batarya Yüzdesi (Sağ Üst)
  tft.fillRect(240, 5, 70, 20, ILI9341_NAVY); 
  tft.setCursor(245, 7); tft.setTextSize(2);
  if (pilYuzdesi < 20) tft.setTextColor(ILI9341_RED);
  else if (pilYuzdesi < 50) tft.setTextColor(ILI9341_YELLOW);
  else tft.setTextColor(ILI9341_GREEN);
  tft.print("%"); tft.print(pilYuzdesi);

  // 2. Sensör 1 
  tft.fillRect(10, 75, 140, 40, ILI9341_BLACK); 
  tft.setCursor(15, 75); tft.setTextSize(4); 
  if(temp1 < -100 || temp1 > 800) { tft.setTextColor(ILI9341_RED); tft.print("HATA"); } 
  else { tft.setTextColor(ILI9341_GREEN); tft.print(temp1, 1); tft.setTextSize(2); tft.print((char)247); tft.print("C"); }

  // 3. Sensör 2 
  tft.fillRect(170, 75, 140, 40, ILI9341_BLACK);
  tft.setCursor(175, 75); tft.setTextSize(4);
  if(temp2 < -100 || temp2 > 800) { tft.setTextColor(ILI9341_RED); tft.print("HATA"); } 
  else { tft.setTextColor(ILI9341_CYAN); tft.print(temp2, 1); tft.setTextSize(2); tft.print((char)247); tft.print("C"); }

  // 4. Akım Değeri 
  tft.fillRect(50, 185, 260, 45, ILI9341_BLACK); 
  tft.setCursor(60, 185); tft.setTextSize(5); tft.setTextColor(ILI9341_YELLOW);
  tft.print(current_mA, 2); 
  tft.setTextSize(2); tft.setTextColor(ILI9341_ORANGE); tft.print(" mA");         

  delay(1000);
}

// --- ARAYÜZ TASARIMI ---
void arayuzuCiz() {
  tft.fillScreen(ILI9341_BLACK);
  
  // Üst Bant
  tft.fillRect(0, 0, 320, 30, ILI9341_NAVY);
  tft.setCursor(10, 7); tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2);
  tft.print("BREN");

  // Çizgiler 
  tft.drawLine(160, 30, 160, 140, ILI9341_DARKGREY); 
  tft.drawLine(0, 140, 320, 140, ILI9341_WHITE);     

  // Başlıklar
  tft.setTextSize(2); tft.setTextColor(ILI9341_LIGHTGREY);

  tft.setCursor(35, 45); tft.print("SENSOR 1");
  tft.setCursor(195, 45); tft.print("SENSOR 2");

  // Akım Başlığı 
  tft.setTextColor(ILI9341_ORANGE);
  tft.setCursor(130, 160); tft.print("AKIM"); 
}