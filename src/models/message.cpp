#include "message.h"
#include <HardwareSerial.h>

Message::Message() { ok = false; }

Message::Message(String &mesg) {
  int length = atoi(mesg.begin());
  if (!length) {
    ok = false;
    return;
  }

  type = mesg.substring(1, length + 1);
  data = mesg.substring(length + 1);
}

Message::Message(String type, String data) {
  this->type = type;
  this->data = data;
}

String Message::encode() { return String(type.length()) + type + data; }