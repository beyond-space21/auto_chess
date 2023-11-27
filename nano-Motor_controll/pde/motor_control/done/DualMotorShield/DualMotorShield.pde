#include <Wire.h>

#define STEPPER1_DIR_PIN 3
#define STEPPER1_STEP_PIN 2
#define STEPPER2_DIR_PIN 7
#define STEPPER2_STEP_PIN 6

#define lm1 A3
#define lm2 A2

#define mag A1
bool st = 0;

int x = 0;
int y = 0;
int ist = 0;

#define MAX_BUF (64)
char buffer[MAX_BUF];
int sofar;
int st1 = 0, st2 = 0;
float px = 0, py = 0;
int step_delay = 10;

void setup() {
  pinMode(STEPPER1_DIR_PIN, OUTPUT);
  pinMode(STEPPER1_STEP_PIN, OUTPUT);
  pinMode(STEPPER2_DIR_PIN, OUTPUT);
  pinMode(STEPPER2_STEP_PIN, OUTPUT);

  pinMode(lm1, INPUT);
  pinMode(lm2, INPUT);

  pinMode(mag, OUTPUT);
  digitalWrite(mag, st);

  Wire.begin(0x8);
  Wire.onReceive(receiveEvent);

  Serial.begin(57600);

  home();
  line(700, 105);
  px = 0, py = 0;
}

int conv(int val, int val1) {
  return ((val << 8) | (val1 & 255));
}
int dta[5];
int ind = 0;
void receiveEvent(int howMany) {
  // Serial.println(howMany);
  if (howMany > 2) {
    dta[5];
    ind = 0;
    while (Wire.available() && ind < 5) {
      dta[ind] = Wire.read();
      // Serial.println(dta[ind]);
      ind++;
    }
    x = conv(dta[1], dta[2]);
    y = conv(dta[3], dta[4]);

    // line(x, y);
    ist = 1;
  } else {
    while (Wire.available()) {
      int a = Wire.read();
    }
    st = !st;
    digitalWrite(mag, st);
    Serial.print("mag ");
    Serial.println(st);
  }
}

void loop() {
  if (ist) {
    Serial.print(x);
    Serial.print(' ');
    Serial.println(y);
    line(x, y);
  }
  Serial.println(ist);
  ist = 0;
  delay(2000);
}

void move1(int d) {
  step_delay = st ? 2 : 2;
  st1 = !st1;
  digitalWrite(STEPPER1_DIR_PIN, d);
  digitalWrite(STEPPER1_STEP_PIN, st1);
  delay(step_delay);
}
void move2(int d) {
  step_delay = st ? 2 : 2;
  st2 = !st2;
  digitalWrite(STEPPER2_DIR_PIN, d);
  digitalWrite(STEPPER2_STEP_PIN, st2);
  delay(step_delay);
}

void line(float newx, float newy) {
  long i;
  long over = 0;

  long dx = newx - px;
  long dy = newy - py;
  int dirx = dx > 0 ? 0 : 1;
  // #ifdef INVERT_Y
  int diry = dy > 0 ? 1 : 0;  // because the motors are mounted in opposite directions
  // Serial.print(dirx);
  // Serial.print(' ');
  // Serial.println(diry);
  // #else
  // int diry = dy>0?1:0;
  // #endif
  dx = abs(dx);
  dy = abs(dy);

  if (dx > dy) {
    over = dx / 2;
    for (i = 0; i < dx; ++i) {
      move1(dirx);
      over += dy;
      if (over >= dx) {
        over -= dx;
        move2(diry);
      }
      // delay(step_delay);
    }
  } else {
    over = dy / 2;
    for (i = 0; i < dy; ++i) {
      move2(diry);
      over += dx;
      if (over >= dy) {
        over -= dy;
        move1(dirx);
      }
      // delay(step_delay);
    }
  }

  px = newx;
  py = newy;
  Serial.print(px);
  Serial.print(' ');
  Serial.println(py);
}

void home() {
  while (!digitalRead(lm1) || !digitalRead(lm2)) {
    if (!digitalRead(lm1))
      move1(1);
    if (!digitalRead(lm2))
      move2(0);
  }
}


void showcase() {

  // a
  line(2000, 1300);
  st = !st;
  digitalWrite(mag, st);
  Serial.print("mag ");
  Serial.println(st);
  line(2000,0);
  st = !st;
  digitalWrite(mag, st);
  Serial.print("mag ");
  Serial.println(st);

// b
  line(2600,2000);
  st = !st;
  digitalWrite(mag, st);
  Serial.print("mag ");
  Serial.println(st);
  line(4500,0);
  st = !st;
  digitalWrite(mag, st);
  Serial.print("mag ");
  Serial.println(st);

}