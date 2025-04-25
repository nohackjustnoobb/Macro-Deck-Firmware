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

Message::Message(String type, std::vector<String> data) {
  String str;
  for (int i = 0; i < data.size(); i++) {
    if (i)
      str += ' ';

    str += data[i];
  }

  this->type = type;
  this->data = str;
}

std::vector<String> Message::parse() {
  std::vector<String> data;

  String temp;
  for (char c : this->data) {
    if (c == ' ') {
      data.push_back(String(temp));
      temp.clear();
      continue;
    }

    temp.concat(c);
  }

  if (temp.length() > 0)
    data.push_back(String(temp));

  return data;
}

String Message::encode() { return String(type.length()) + type + data; }