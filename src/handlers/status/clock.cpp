#include "fonts/test.h"
#include "handlers/status/module.h"
#include "models/common.h"
#include "models/constants.h"

#include <ESP32Time.h>
#include <TFT_eSPI.h>

class ClockModule : public Module {
public:
  ClockModule() { name = "clock"; }

  int16_t textColor = TFT_WHITE;
  int16_t bgColor = TFT_BLACK;

  void draw() {
    char t_buf[100];
    TFT_eSprite sprite = TFT_eSprite(&tft);
    int16_t timeTextSize = width / 120;
    int16_t dateTextSize = timeTextSize - 2;
    if (timeTextSize <= 2)
      dateTextSize = 2;

    int16_t yoffset = (statusBarHeight - (timeTextSize + dateTextSize) * 8) / 2;

    sprite.createSprite(width, statusBarHeight);
    sprite.fillRect(0, 0, width, statusBarHeight, bgColor);

    sprite.setTextDatum(TC_DATUM);
    // TODO Font config
    //  sprite.loadFont(test);
    sprite.setTextSize(timeTextSize);
    sprite.setTextColor(textColor, bgColor);

    sprintf(t_buf, "%02d:%02d:%02d", rtc.getHour(true), rtc.getMinute(),
            rtc.getSecond());
    sprite.drawString(t_buf, width / 2, yoffset);

    sprite.setTextSize(dateTextSize);
    sprintf(t_buf, "%04d-%02d-%02d", rtc.getYear(), rtc.getMonth() + 1,
            rtc.getDay());
    sprite.drawString(t_buf, width / 2, yoffset + timeTextSize * 8);

    sprite.pushSprite(0, GAP_SIZE + buttonsHeight);
  }

private:
};