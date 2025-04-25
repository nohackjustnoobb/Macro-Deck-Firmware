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
    if (selected.isEmpty())
      return;

    // FIXME dont know why it is called when selected is cleared.
    // Maybe the tft have delay and not blocking.

    // for (auto module : modules) {
    //   if (module->name != selected)
    //     continue;
    //   module->draw();
    // }
  }

  bool is(String &type) { return type == "ss"; }

  bool handle(Message &mesg) {
    std::vector<String> data = mesg.parse();

    int16_t x, y;
    int size;

    x = data[0].toInt();
    y = data[1].toInt();
    size = data[2].toInt();

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

    if (result)
      selected.clear();

    return false;
  }

  bool is(uint16_t &x, uint16_t &y) { return x <= width && y >= buttonsHeight; }

  bool handle(uint16_t &x, uint16_t &y) {
    Serial.println(Message("sc", String(x)).encode());

    return false;
  }

private:
  std::vector<Module *> modules; // List of modules to draw in the status bar
  String selected;               // selected for status bar
};