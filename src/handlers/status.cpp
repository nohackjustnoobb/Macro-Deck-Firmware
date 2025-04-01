#include <TJpg_Decoder.h>
#include <sstream>

#include "../models/constants.h"
#include "../models/handler.h"

class StatusManager : public Handler {
public:
  StatusManager() {}

  StatusManager(int16_t offset, int16_t *height, int16_t *width) {
    _height = height;
    _width = width;
    _offset = offset;
  }

  bool is(String &type) { return type == "ss"; }

  bool handle(Message &mesg) {
    int16_t x, y;
    int size;

    // TODO remove the usage of sstream to reduce the size of the binary
    std::istringstream iss(mesg.data.c_str());
    iss >> x;
    iss >> y;
    iss >> size;

    if (size <= 0) {
      Serial.println(NOT_OK);
      return false;
    }

    Serial.println(RD);

    char buf[size];
    Serial.readBytes(buf, size);

    JRESULT result =
        TJpgDec.drawJpg(x, y + _offset + GAP_SIZE, (const uint8_t *)buf, size);
    Serial.println(result ? NOT_OK : OK);

    return false;
  }

  bool is(uint16_t &x, uint16_t &y) { return x <= *_width && y >= _offset; }

  bool handle(uint16_t &x, uint16_t &y) {
    Serial.println(Message("sc", String(x)).encode());

    return false;
  }

private:
  int16_t *_height;
  int16_t *_width;
  int16_t _offset;
};