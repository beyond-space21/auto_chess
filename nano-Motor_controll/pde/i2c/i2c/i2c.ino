#include <Wire.h>

void setup() {
  Wire.begin(0x8);         
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
}

int conv(int val, int val1) {
  return ((val << 8) | (val1 & 255));
}

void receiveEvent(int howMany) {
    // Serial.println(howMany);
    int dta[5];
    int ind=0;
  while (Wire.available() && ind<5) {
    dta[ind] = Wire.read();
    // Serial.println(dta[ind]);
    ind++;
  }
  int x = conv(dta[1],dta[2]);
  int y = conv(dta[3],dta[4]);

  Serial.print(x);
  Serial.print(' ');
  Serial.println(y);

}
void loop() {
  delay(100);
}