#include "message.h"
#include <HardwareSerial.h>
#include <sstream>

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
  // TODO remove the usage of sstream to reduce the size of the binary

  std::stringstream ss(data.c_str());
  String word;
  std::vector<String> words;

  while (ss >> word.begin()) {
    words.push_back(word);
  }

  return words;
}

String Message::encode() { return String(type.length()) + type + data; }