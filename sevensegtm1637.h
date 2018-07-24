#include <locale>
#include <Arduino.h>

using namespace std;

class SevenSegmentTM1637 {

  private:
    int clockPin = 22;
    int dataPin = 23;

    // 0-9
    uint8_t digits[10] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f };
    uint8_t chars[256];

    void initChars();
    String text = "";
    int scrollInitMS = -1;
    int scrollDelayMS = 300;
    int lastOffset = 0;

    void start(void);
    void stop(void);
    bool writeValue(uint8_t value);

  public:
    uint8_t SEG_TOP_LEFT = B00100000;
    uint8_t SEG_BTM_LEFT = B00010000;
    uint8_t SEG_TOP =      B00000001;
    uint8_t SEG_MID =      B01000000;
    uint8_t SEG_BTM =      B00001000;
    uint8_t SEG_TOP_RIGHT =B00000010;
    uint8_t SEG_BTM_RIGHT =B00000100;
    uint8_t ALL_SEGMENTS = B01111111;

    SevenSegmentTM1637(int, int);
    void init(); // call in setup();
    void setScrollDelayMS(int ms);
    void updateScroll();

    // can mostly show 0-9 a-z
    void showText(String msg);
    void scrollText(String message);
    void showSegments(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth);
};


