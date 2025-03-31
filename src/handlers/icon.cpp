#include <TJpg_Decoder.h>

#include "../models/constants.h"
#include "../models/handler.h"

class IconManager : public Handler {
public:
  IconManager() {}

  IconManager(int16_t *height, int16_t *width) {
    _height = height;
    _width = width;

    for (int16_t i = 0; i < BUTTONS_PER_ROW * NUM_OF_ROWS; i++)
      draw(i);
  }

  bool is(String &type) {
    return type.startsWith("wi") || type.startsWith("ri");
  }

  void handleWI(Message &mesg) {
    int16_t idx = mesg.type.substring(2).toInt();
    int size = mesg.data.toInt();

    if (size <= 0)
      return;

    Serial.println(RD);

    char buf[size];
    int replaced = Serial.readBytes(buf, size);

    write(idx, buf, size);
  }

  void handleRI(Message &mesg) {
    int16_t idx = mesg.type.substring(2).toInt();

    File file =
        SD.open(("/" + std::to_string(idx) + ".jpg").c_str(), FILE_READ);

    if (!file) {
      Serial.println(OK);
      return;
    }

    Serial.println(Message(String("rd?"), String(file.size())).encode());

    while (Serial.available() <= 0) {
    }

    String reply = Serial.readString();
    if (reply != RD)
      return;

    while (file.available())
      Serial.write(file.read());
  }

  bool handle(Message &mesg) {
    if (mesg.type.startsWith("wi"))
      handleWI(mesg);

    if (mesg.type.startsWith("ri"))
      handleRI(mesg);

    return false;
  }

  bool is(uint16_t &x, uint16_t &y) { return x <= *_width && y <= *_height; }

  bool handle(uint16_t &x, uint16_t &y) {
    int16_t row = y / (*_height / NUM_OF_ROWS);
    int16_t col = x / (*_width / BUTTONS_PER_ROW);

    Serial.println(
        Message("ic" + String(row * BUTTONS_PER_ROW + col), "").encode());

    return false;
  }

  void draw(int16_t idx) {
    int16_t row = idx / BUTTONS_PER_ROW;
    int16_t col = idx % BUTTONS_PER_ROW;
    int16_t x = col * (*_width / BUTTONS_PER_ROW + GAP_SIZE / BUTTONS_PER_ROW);
    int16_t y = row * (*_height / NUM_OF_ROWS + GAP_SIZE / NUM_OF_ROWS);

    TJpgDec.drawSdJpg(x, y, ("/" + std::to_string(idx) + ".jpg").c_str());
  }

  void write(int16_t idx, const char *buf, int size) {
    File file =
        SD.open(("/" + std::to_string(idx) + ".jpg").c_str(), FILE_WRITE);
    if (!file) {
      Serial.println(NOT_OK);
      return;
    }

    if (file.write((const uint8_t *)buf, size))
      Serial.println(OK);
    else
      Serial.println(NOT_OK);

    file.close();

    draw(idx);
  }

private:
  int16_t *_height;
  int16_t *_width;
};