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
    std::vector<String> info = {String(width), String(height),
                                String(BUTTONS_PER_ROW), String(NUM_OF_ROWS),
                                String(GAP_SIZE)};

    Serial.println(Message("li", info).encode());
  }

  void handleST(int epoch) {
    rtc.setTime(epoch);
    Serial.println(OK);
  }

private:
};