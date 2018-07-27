#include "sevensegtm1637.h"
#include <stdio.h>

SevenSegmentTM1637 led1(22, 23);
SevenSegmentTM1637 led2(24, 25);


void setup() {
  led1.init();
  led2.init();

  led1.showText("rpm");
  led2.showText("2.5");
}

void loop() {
  float degrees = 0;
  do {
    char buf[5];
    snprintf(buf, 5, "%.1f", degrees);
    led2.showText(String(buf));
    delay(30);
    degrees += 0.1;
  } while (degrees < 30.0);
}

