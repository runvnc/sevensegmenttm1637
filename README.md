Simple Arduino class for TM1637 based on [moozzyk's code](https://github.com/moozzyk/TM1637Clock).
Can show text and also scrolling text.

```c++
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
```
