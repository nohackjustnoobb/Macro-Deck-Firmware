#pragma once
#include <WString.h>

class Message {
public:
  String type;
  String data;
  bool ok = true;

  Message();

  Message(String &mesg);

  Message(String type, String data);

  String encode();
};