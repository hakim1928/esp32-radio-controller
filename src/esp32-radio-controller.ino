*
  Radiocommande CREATIC - ESP32
  - OLED SSD1306 (I2C SDA=21, SCL=22)
  - 2 KY-040 (AiEsp32RotaryEncoder)
  - 4 Joysticks analogiques (35,34,32,33)
  - 6 Switches (0,2,15,4,14,36)
  - nRF24L01 (CE=17, CSN=5, SCK=18, MOSI=23, MISO=19)
  Version: v1.1
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AiEsp32RotaryEncoder.h>
#include <SPI.h>
#include <RF24.h>

// ================= CONFIG =================
#define VERSION "v1.1"

// OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// nRF24L01
#define NRF_CE_PIN 17
#define NRF_CSN_PIN 5
#define NRF_SCK_PIN 18
#define NRF_MOSI_PIN 23
#define NRF_MISO_PIN 19
RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

// Encodeur 1 (menu)
#define ENC1_CLK 25
#define ENC1_DT  26
#define ENC1_SW  27
AiEsp32RotaryEncoder encoder1(ENC1_CLK, ENC1_DT, ENC1_SW, -1);

// Encodeur 2 (affichage page 4)
#define ENC2_CLK 16
#define ENC2_DT  12
#define ENC2_SW  13
AiEsp32RotaryEncoder encoder2(ENC2_CLK, ENC2_DT, ENC2_SW, -1);
long encoder2Value = 0;
bool encoder2Button = false;

// Joysticks
#define LEFT_V_PIN  35
#define LEFT_H_PIN  34
#define RIGHT_V_PIN 32
#define RIGHT_H_PIN 33

// Switches
#define SW_A 0
#define SW_B 2
#define SW_C 15
#define SW_D 4
#define SW_E 14
#define SW_F 36

// Pages
enum Pages { PAGE_ACCUEIL=1, PAGE_JOYSTICKS=2, PAGE_SWITCHES=3, PAGE_ENCODER2=4 };
int currentPage = PAGE_ACCUEIL;
const int totalPages = 4;

// Forward declarations
void afficherAccueil();
void afficherMenuBar();
void afficherJoysticks();
void afficherSwitches();
void afficherEncoder2();
void afficherPage();

// ===== helper =====
float mapToDegrees(int raw) {
  return (raw / 4095.0f) * 360.0f - 180.0f;
}

// ===== Encoder callbacks =====
void IRAM_ATTR isr_enc1() { encoder1.readEncoder_ISR(); }
void IRAM_ATTR isr_enc2() { encoder2.readEncoder_ISR(); }

// ===== Handlers =====
void handleEncoder1() {
  encoder1.tick();

  if (encoder1.isEncoderButtonClicked()) {
    currentPage = PAGE_ACCUEIL;
    encoder1.setEncoderValue(currentPage);
    afficherPage();
    delay(200);
    return;
  }

  int val = encoder1.readEncoder();
  if (val != currentPage) {
    if (val < PAGE_ACCUEIL) val = totalPages;
    if (val > totalPages) val = PAGE_ACCUEIL;
    currentPage = val;
    encoder1.setEncoderValue(currentPage);
    afficherPage();
  }
}

void handleEncoder2() {
  encoder2.tick();
  long newVal = encoder2.readEncoder();
  if (newVal != encoder2Value) {
    encoder2Value = newVal;
  }
  if (encoder2.isEncoderButtonClicked()) {
    encoder2Button = !encoder2Button;
  }
}

// ===== Display/menu =====
void afficherMenuBar() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("[1 Acc] [2 Joy] [3 Sw] [4 Enc2]");
  int xStart = (currentPage - 1) * 32;
  display.drawLine(xStart, 10, xStart + 28, 10, SSD1306_WHITE);
}

void afficherPage() {
  switch (currentPage) {
    case PAGE_ACCUEIL: afficherAccueil(); break;
    case PAGE_JOYSTICKS: afficherJoysticks(); break;
    case PAGE_SWITCHES: afficherSwitches(); break;
    case PAGE_ENCODER2: afficherEncoder2(); break;
  }
}

// ===== Pages =====
void afficherAccueil() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 6);
  display.println("RADIOCOMMANDE");

  String titre = "CREATIC";
  display.setTextSize(2);
  int16_t x1,y1; uint16_t w,h;
  display.getTextBounds(titre, 0, 0, &x1, &y1, &w, &h);
  int x = (SCREEN_WIDTH - w) / 2;
  int y = 18;

  for (int i = 0; i <= titre.length(); i++) {
    display.fillRect(0, 16, SCREEN_WIDTH, 32, SSD1306_BLACK);
    display.setCursor(x, y);
    display.print(titre.substring(0, i));
    display.display();
    delay(80);
  }

  display.setTextSize(1);
  display.setCursor(SCREEN_WIDTH - 30, SCREEN_HEIGHT - 10);
  display.print(VERSION);
  display.display();
}

void afficherJoysticks() {
  int lv = analogRead(LEFT_V_PIN);
  int lh = analogRead(LEFT_H_PIN);
  int rv = analogRead(RIGHT_V_PIN);
  int rh = analogRead(RIGHT_H_PIN);

  float lvd = mapToDegrees(lv);
  float lhd = mapToDegrees(lh);
  float rvd = mapToDegrees(rv);
  float rhd = mapToDegrees(rh);

  display.clearDisplay();
  afficherMenuBar();
  display.drawLine(0, 14, SCREEN_WIDTH-1, 14, SSD1306_WHITE);
  display.drawLine(SCREEN_WIDTH/2, 14, SCREEN_WIDTH/2, SCREEN_HEIGHT-1, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(4, 18);
  display.printf("L_V:%4d\nDeg:%6.1f", lv, lvd);
  display.setCursor(4, 44);
  display.printf("L_H:%4d\nDeg:%6.1f", lh, lhd);
  display.setCursor(SCREEN_WIDTH/2+4, 18);
  display.printf("R_V:%4d\nDeg:%6.1f", rv, rvd);
  display.setCursor(SCREEN_WIDTH/2+4, 44);
  display.printf("R_H:%4d\nDeg:%6.1f", rh, rhd);

  display.display();
}

void afficherSwitches() {
  int s0 = digitalRead(SW_A);
  int s1 = digitalRead(SW_B);
  int s2 = digitalRead(SW_C);
  int s3 = digitalRead(SW_D);
  int s4 = digitalRead(SW_E);
  int s5 = digitalRead(SW_F);

  bool swA_on = (s0 == LOW);
  bool swB_on = (s1 == LOW);
  bool swC_on = (s2 == HIGH);
  bool swD_on = (s3 == LOW);
  bool swE_on = (s4 == LOW);
  bool swF_on = (s5 == LOW);

  display.clearDisplay();
  afficherMenuBar();
  display.setTextSize(1);
  display.setCursor(4, 18);
  display.printf("0:%s  2:%s 15:%s", swA_on?"ON":"OFF", swB_on?"ON":"OFF", swC_on?"ON":"OFF");
  display.setCursor(4, 36);
  display.printf("4:%s 14:%s 36:%s", swD_on?"ON":"OFF", swE_on?"ON":"OFF", swF_on?"ON":"OFF");
  display.display();
}

void afficherEncoder2() {
  display.clearDisplay();
  afficherMenuBar();

  display.setTextSize(2);
  display.setCursor(10, 24);
  display.print("Val: ");
  display.print(encoder2Value);

  display.setTextSize(1);
  display.setCursor(10, 50);
  display.print("Bouton: ");
  display.print(encoder2Button ? "PRESSE" : "RELACHE");

  display.display();
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  analogReadResolution(12);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Erreur OLED");
    while (1) delay(1000);
  }
  display.clearDisplay();

  SPI.begin(NRF_SCK_PIN, NRF_MISO_PIN, NRF_MOSI_PIN, NRF_CSN_PIN);
  radio.begin();

  encoder1.begin();
  encoder1.setup(isr_enc1);
  encoder1.setBoundaries(PAGE_ACCUEIL, totalPages, true);
  encoder1.setEncoderValue(currentPage);

  encoder2.begin();
  encoder2.setup(isr_enc2);

  pinMode(SW_A, INPUT_PULLUP);
  pinMode(SW_B, INPUT_PULLUP);
  pinMode(SW_C, INPUT_PULLDOWN);
  pinMode(SW_D, INPUT_PULLUP);
  pinMode(SW_E, INPUT_PULLUP);
  pinMode(SW_F, INPUT);

  afficherAccueil();
}

// ===== Loop =====
void loop() {
  handleEncoder1();
  handleEncoder2();

  if (currentPage == PAGE_JOYSTICKS) afficherJoysticks();
  else if (currentPage == PAGE_SWITCHES) afficherSwitches();
  else if (currentPage == PAGE_ENCODER2) afficherEncoder2();

  delay(120);
}
