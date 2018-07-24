#include "sevensegtm1637.h"

SevenSegmentTM1637 ledDisplay(22, 23);

void setup() {
  ledDisplay.init();
  ledDisplay.showText("Init");
  delay(1000);
  ledDisplay.scrollText("0123456789  abcdefghijklmnopqrstuvwyz");
}

void loop() {
  ledDisplay.updateScroll();
}

