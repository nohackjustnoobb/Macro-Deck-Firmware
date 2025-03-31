#include <HardwareSerial.h>

#include "../models/constants.h"
#include "../models/handler.h"

class InfoHandler : public Handler {
public:
  InfoHandler(int16_t *height, int16_t *width) {
    _height = height;
    _width = width;
  }

  bool is(String &type) { return type == "li"; }

  bool handle(Message &mesg) {
    int16_t infos[] = {*_width, *_height, BUTTONS_PER_ROW, NUM_OF_ROWS,
                       GAP_SIZE};

    String str;
    for (int i = 0; i < sizeof(infos) / sizeof(int16_t); i++) {
      if (infos[i])
        str += ' ';

      str += String(infos[i]);
    }

    Serial.println(Message("li", str).encode());

    return false;
  }

private:
  int16_t *_height;
  int16_t *_width;
};