Simple Arduino class for TM1637 based on [moozzyk's code](https://github.com/moozzyk/TM1637Clock).
Can show text and also scrolling text.

If it is string representing a floating point number then it will show the decimal
point correctly and right align. (You can use snprintf to format with a certain
number of decimal places before calling showText.)

```c++
#include "sevensegtm1637.h"

const int clockPin = 22;
const int dataPin = 23;

SevenSegmentTM1637 ledDisplay(clockPin, dataPin);

void setup() {
  ledDisplay.init();
  ledDisplay.showText("Init");
  delay(1000);
  ledDisplay.scrollText("0123456789  abcdefghijklmnopqrstuvwyz");
}

void loop() {
  ledDisplay.updateScroll();
}
```
