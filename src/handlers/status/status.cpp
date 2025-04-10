#include <TJpg_Decoder.h>
#include <sstream>
#include <vector>

#include "../../models/constants.h"
#include "../../models/handler.h"
#include "clock.cpp"

class StatusManager : public Handler {
public:
  StatusManager() {
    modules.push_back(new ClockModule());
    selected = "clock";
  }

  void handle() {
    for (auto module : modules) {
      if (module->name != selected)
        continue;
      module->draw();
    }
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

    JRESULT result = TJpgDec.drawJpg(x, y + buttonsHeight + GAP_SIZE,
                                     (const uint8_t *)buf, size);
    Serial.println(result ? NOT_OK : OK);

    // Reset selected after handling the message
    selected = "";

    return false;
  }

  bool is(uint16_t &x, uint16_t &y) { return x <= width && y >= buttonsHeight; }

  bool handle(uint16_t &x, uint16_t &y) {
    Serial.println(Message("sc", String(x)).encode());

    return false;
  }

private:
  std::vector<Module *> modules; // List of modules to draw in the status bar
  String selected;               // selected for status bar, if needed
};