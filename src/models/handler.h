#pragma once
#include "message.h"

class Handler {
public:
  virtual void handle() {};

  virtual bool is(String &type) { return false; };

  virtual bool handle(Message &mesg) { return false; };

  virtual bool is(uint16_t &x, uint16_t &y) { return false; };

  virtual bool handle(uint16_t &x, uint16_t &y) { return false; };
};