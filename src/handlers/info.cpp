#include <HardwareSerial.h>

#include "../models/common.h"
#include "../models/constants.h"
#include "../models/handler.h"
class InfoHandler : public Handler {
public:
  bool is(String &type) { return type == "li" || type == "st"; }

  bool handle(Message &mesg) {
    switch (mesg.type[0]) {
    case 'l': // li
      handleLI();
      break;
    case 's': // st
      handleST(mesg.data.toInt());
      break;
    }

    return false;
  }

  void handleLI() {
    int16_t infos[] = {width, height, BUTTONS_PER_ROW, NUM_OF_ROWS, GAP_SIZE};

    String str;
    for (int i = 0; i < sizeof(infos) / sizeof(int16_t); i++) {
      if (infos[i])
        str += ' ';

      str += String(infos[i]);
    }

    Serial.println(Message("li", str).encode());
  }
  void handleST(int epoch) {
    rtc.setTime(epoch);
    Serial.println(OK);
  }

private:
};