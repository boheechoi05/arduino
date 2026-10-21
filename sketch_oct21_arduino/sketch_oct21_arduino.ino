#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte POT_PIN = A0;  // 가변저항 가운데 핀

byte dog0[] = {B00000, B10110, B01111, B01111, B01110, B10001, B10001, B00000};
byte dog1[] = {B00000, B00110, B11111, B01111, B01110, B10001, B10001, B00000};
byte dog2[] = {B00000, B10110, B01111, B01111, B01110, B01010, B01010, B01010};
byte dog3[] = {B00000, B00110, B11111, B01111, B01110, B01010, B01010, B01010};
byte dog4[] = {B00000, B10110, B01111, B01111, B01110, B00100, B01010, B00000};
byte dog5[] = {B00000, B00110, B11111, B01111, B01110, B00100, B01010, B00000};

const byte seq[] = {0, 2, 4, 5, 3, 1};
const byte SEQ_LEN = sizeof(seq);

byte frame = 0;   // 다리 동작 프레임
byte pos = 0;     // 강아지 위치 (0 ~ 15)

unsigned long lastFrame = 0;
unsigned long lastMove = 0;
unsigned long frameInterval = 200;  // 다리 바뀌는 간격(ms), 가변저항으로 변함
unsigned long moveInterval = 400;   // 한 칸 이동 간격(ms), 가변저항으로 변함

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.createChar(0, dog0);
  lcd.createChar(1, dog1);
  lcd.createChar(2, dog2);
  lcd.createChar(3, dog3);
  lcd.createChar(4, dog4);
  lcd.createChar(5, dog5);

  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("My dog Teddy!");
}

void loop() {
  // 1) 가변저항 값 읽어서 속도 정하기
  int pot = analogRead(POT_PIN);                  // 0 ~ 1023
  moveInterval = map(pot, 0, 1023, 700, 100);     // 돌릴수록 빨라짐
  frameInterval = moveInterval / 2;               // 다리는 이동의 2배 빠르게

  unsigned long now = millis();

  // 2) 한 칸 이동
  if (now - lastMove >= moveInterval) {
    lastMove = now;

    lcd.setCursor(pos, 0);
    lcd.print(' ');            // 이전 위치 지우기

    pos++;
    if (pos > 15) pos = 0;     // 끝에 도착하면 처음으로

    lcd.setCursor(pos, 0);
    lcd.write(seq[frame]);     // 새 위치에 바로 그리기
  }

  // 3) 다리 동작 바꾸기
  if (now - lastFrame >= frameInterval) {
    lastFrame = now;

    frame++;
    if (frame >= SEQ_LEN) frame = 0;

    lcd.setCursor(pos, 0);
    lcd.write(seq[frame]);
  }
}