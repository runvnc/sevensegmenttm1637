#include <locale>
#include "sevensegtm1637.h"

SevenSegmentTM1637::SevenSegmentTM1637(int clockPin_, int dataPin_) {
  clockPin = clockPin_;
  dataPin = dataPin_;
}

void SevenSegmentTM1637::initChars() {
  chars[' '] = 0;
  for (int i=0; i<256; i++)
    chars[i] = chars[' '];
  chars['e'] = ALL_SEGMENTS & ~(SEG_TOP_RIGHT | SEG_BTM_RIGHT);
  chars['0'] = digits[0];
  chars['1'] = digits[1];
  chars['2'] = digits[2];
  chars['3'] = digits[3];
  chars['4'] = digits[4];
  chars['5'] = digits[5];
  chars['6'] = digits[6];
  chars['7'] = digits[7];
  chars['8'] = digits[8];
  chars['9'] = digits[9];
  chars['.'] = SEG_DECIMAL_PT;
  chars['6'] = ALL_SEGMENTS & ~SEG_TOP_RIGHT;
  chars['a'] = ALL_SEGMENTS & ~SEG_BTM;
  chars['b'] = SEG_BTM_LEFT | SEG_BTM | SEG_BTM_RIGHT | SEG_MID | SEG_TOP_LEFT;
  chars['c'] = SEG_MID | SEG_BTM_LEFT | SEG_BTM;
  chars['d'] = ALL_SEGMENTS & ~(SEG_TOP_LEFT | SEG_TOP);
  chars['o'] = SEG_MID | SEG_BTM_LEFT | SEG_BTM | SEG_BTM_RIGHT;
  chars['t'] = SEG_TOP_LEFT | SEG_BTM_LEFT | SEG_MID | SEG_BTM;
  chars['f'] = SEG_TOP | SEG_TOP_LEFT | SEG_BTM_LEFT | SEG_MID;
  chars['g'] = chars['6'] & ~SEG_MID;
  chars['h'] = SEG_TOP_LEFT | SEG_BTM_LEFT | SEG_MID | SEG_BTM_RIGHT;
  chars['i'] = SEG_BTM_LEFT;
  chars['j'] = SEG_TOP_RIGHT | SEG_BTM_RIGHT | SEG_BTM | SEG_BTM_LEFT;
  chars['k'] = SEG_TOP_LEFT | SEG_BTM_LEFT | SEG_MID | SEG_BTM | SEG_TOP_RIGHT;
  chars['l'] = SEG_TOP_LEFT | SEG_BTM_LEFT | SEG_BTM;
  chars['m'] = SEG_BTM_LEFT | SEG_MID | SEG_BTM_RIGHT;
  chars['n'] = SEG_BTM_LEFT | SEG_MID | SEG_BTM_RIGHT;
  chars['p'] = SEG_TOP_LEFT | SEG_TOP | SEG_TOP_RIGHT | SEG_MID | SEG_BTM_LEFT;
  chars['q'] = SEG_TOP_LEFT | SEG_TOP | SEG_TOP_RIGHT | SEG_MID | SEG_BTM_RIGHT;
  chars['r'] = SEG_BTM_LEFT | SEG_MID;
  chars['s'] = SEG_TOP | SEG_TOP_LEFT | SEG_MID | SEG_BTM_RIGHT | SEG_BTM;
  chars['v'] = SEG_BTM_LEFT | SEG_BTM | SEG_BTM_RIGHT;
  chars['w'] = SEG_BTM_LEFT | SEG_BTM | SEG_BTM_RIGHT;
  chars['x'] = ALL_SEGMENTS & ~(SEG_TOP | SEG_BTM);
  chars['y'] = SEG_TOP_LEFT | SEG_MID | SEG_TOP_RIGHT | SEG_BTM_RIGHT;
  chars['z'] = SEG_TOP | SEG_TOP_RIGHT | SEG_MID | SEG_BTM_LEFT | SEG_BTM;
  chars['u'] = SEG_BTM_LEFT | SEG_BTM | SEG_BTM_RIGHT;
}

void SevenSegmentTM1637::init()
{
  initChars();
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  start();
  writeValue(0x8c);
  stop();

  showSegments(0x00, 0x00, 0x00, 0x00);
}

void SevenSegmentTM1637::setScrollDelayMS(int ms) {
  scrollDelayMS = ms;
}

void SevenSegmentTM1637::updateScroll() {
  int now = millis();
  int elapsed = now - scrollInitMS;
  int offset = elapsed / scrollDelayMS;
  if (offset == lastOffset) return;

  lastOffset = offset;
  int charOffset = offset % text.length();
  showSegments(chars[tolower(text[charOffset])], chars[tolower(text[charOffset+1])], 
        chars[tolower(text[charOffset+2])], chars[tolower(text[charOffset+3])]);
}

void SevenSegmentTM1637::scrollText(String message) {
  text = "    " + message;
  scrollInitMS = millis();
  updateScroll();
}

String SevenSegmentTM1637::padRightAlign(String msg) {
  byte len = msg.length();
  if (msg.indexOf(".") != -1) 
    len--;
  else
    return msg;
  byte toPad = 4-len;
  for (byte i=0; i<toPad; i++)
    msg = " " + msg;
  return msg;
}

void SevenSegmentTM1637::showText(String msg) {
  msg = padRightAlign(msg);
  byte segments[4] = {0,0,0,0}, seg = 0;
  for (int ch=0; ch < msg.length() && seg<=4; ch++) {
    if (msg[ch] == '.' && seg>0)
      segments[seg-1] = segments[seg-1] | SEG_DECIMAL_PT;
    else {
      segments[seg] = chars[tolower(msg[ch])];
      seg++;
    }
  }  
    
  showSegments(segments[0], segments[1], segments[2], segments[3]);
  //showSegments(SEG_DECIMAL_PT, SEG_DECIMAL_PT, SEG_DECIMAL_PT, SEG_DECIMAL_PT);
}

void SevenSegmentTM1637::showSegments(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth)
{
  start();
  writeValue(0x40);
  stop();

  start();
  writeValue(0xc0);
  writeValue(first);
  writeValue(second);
  writeValue(third);
  writeValue(fourth);
  stop();
}

void SevenSegmentTM1637::start(void)
{
  digitalWrite(clockPin,HIGH);//send start signal to TM1637
  digitalWrite(dataPin,HIGH);
  delayMicroseconds(5);

  digitalWrite(dataPin,LOW);
  digitalWrite(clockPin,LOW);
  delayMicroseconds(5);
}

void SevenSegmentTM1637::stop(void)
{
  digitalWrite(clockPin,LOW);
  digitalWrite(dataPin,LOW);
  delayMicroseconds(5);

  digitalWrite(clockPin,HIGH);
  digitalWrite(dataPin,HIGH);
  delayMicroseconds(5);
}

bool SevenSegmentTM1637::writeValue(uint8_t value)
{
  for(uint8_t i = 0; i < 8; i++)
  {
    digitalWrite(clockPin, LOW);
    delayMicroseconds(5);
    digitalWrite(dataPin, (value & (1 << i)) >> i);
    delayMicroseconds(5);
    digitalWrite(clockPin, HIGH);
    delayMicroseconds(5);
  }

  // wait for ACK
  digitalWrite(clockPin,LOW);
  delayMicroseconds(5);

  pinMode(dataPin,INPUT);

  digitalWrite(clockPin,HIGH);
  delayMicroseconds(5);

  bool ack = digitalRead(dataPin) == 0;

  pinMode(dataPin,OUTPUT);

  return ack;
}

