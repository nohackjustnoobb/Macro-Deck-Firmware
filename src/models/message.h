#pragma once
#include <WString.h>
#include <vector>

template <typename T> String join(std::vector<T> datas) {
  String str;

  for (int i = 0; i < datas.size(); i++) {
    if (i)
      str += ' ';

    str += String(datas[i]);
  }

  return str;
}

class Message {
public:
  String type;
  String data;
  bool ok = true;

  Message();

  Message(String &mesg);

  Message(String type, String data);

  std::vector<String> parse();

  String encode();
};