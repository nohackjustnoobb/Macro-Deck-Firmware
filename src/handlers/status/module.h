#pragma once
#include "models/common.h"
#include <WString.h>

class Module {
public:
  String name;
  virtual void draw() = 0;
};