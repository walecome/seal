#pragma once

#include "common/Value.hh"

class SystemRegisters {
  public:
    SystemRegisters();

    void set_return_value(Value& value);
    Value& get_return_value();

  private:
};
