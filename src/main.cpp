#include <TJpg_Decoder.h>
#include <spi.h>
#include <vector>

#include "handlers/icon.cpp"
#include "handlers/info.cpp"
#include "handlers/status/status.cpp"
#include "models/common.h"
#include "models/constants.h"
#include "models/message.h"

// Hardware related
SPIClass _spi(HSPI);
TFT_eSPI tft;
ESP32Time rtc;

bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  if (y >= tft.height())
    return 0;

  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}

// Software related
std::vector<Handler *> handlers;
int16_t height, width, buttonsHeight, statusBarHeight;

void setup() {
  // Initialize Serial
  Serial.begin(BAUDRATE);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  _spi.begin(SD_SCK, SD_MISO, SD_MOSI);

  // Initialise SD before TFT
  while (!SD.begin(SD_CS, _spi)) {
    Serial.println("Error: SD.begin failed");
    delay(1000);
  }

  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setRotation(ROTATION);

  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tftOutput);

  // Initialise variables
  height = tft.height();
  width = tft.width();

  int16_t imgSize =
      (width - GAP_SIZE * (BUTTONS_PER_ROW - 1)) / BUTTONS_PER_ROW;
  buttonsHeight = imgSize * NUM_OF_ROWS + GAP_SIZE * (NUM_OF_ROWS - 1);
  statusBarHeight = height - buttonsHeight - GAP_SIZE;

  // Initialize the ESP32 RTC
  rtc.setTime(0, 0, 0, 4, 6, 1989);

  // Initialise handlers
  handlers.push_back(new IconManager());
  handlers.push_back(new StatusManager());
  handlers.push_back(new InfoHandler());
}

String raw;
Message parsed;
void handleSerial() {
  if (Serial.available() <= 0)
    return;

  // Parse message
  raw = Serial.readStringUntil('\n');
  parsed = Message(raw);
  if (!parsed.ok)
    return;

  // Handle message
  for (auto handler : handlers) {
    if (!handler->is(parsed.type))
      continue;

    bool conti = handler->handle(parsed);
    if (!conti)
      return;
  }
}

unsigned long lastTouch = 0;
uint16_t x, y;
void handleTouch() {
  bool touched = tft.getTouch(&x, &y);
  if (!touched && lastTouch == 0)
    return;

  if (lastTouch == 0 ||
      (touched && millis() - lastTouch < MAX_TOUCH_TIME_DIFF)) {
    lastTouch = millis();
    return;
  }

  lastTouch = 0;

  uint16_t temp;
  switch (ROTATION) {
  case 0:
    temp = x;
    x = height - y;
    y = temp;
    break;
  case 2:
    temp = x;
    x = y;
    y = width - temp;
    break;
  case 3:
    y = height - y;
    x = width - x;
    break;
  }

  // Handle touch
  for (auto handler : handlers) {
    if (!handler->is(x, y))
      continue;

    bool conti = handler->handle(x, y);
    if (!conti)
      return;
  }
}

void loop() {
  handleSerial();
  handleTouch();

  for (auto handler : handlers)
    handler->handle();
}