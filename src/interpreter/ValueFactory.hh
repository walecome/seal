#pragma once

#include "common/Value.hh"
#include "Constants.hh"

class ValueFactory {
  public:
    virtual ~ValueFactory();

    static ptr_t<ValueFactory> for_boolean(bool value);
    static ptr_t<ValueFactory> for_integer(int value);
    static ptr_t<ValueFactory> for_real(double value);
    static ptr_t<ValueFactory> for_string(std::string value);
    static ptr_t<ValueFactory> for_copy(const Value& value);

    Value produce();
};

