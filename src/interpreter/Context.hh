#pragma once

#include "interpreter/Value.hh"

class Context {
  public:
    Value create_value_from(String value) const;
    Value create_value_from(Boolean value) const;
    Value create_value_from(Integer value) const;
    Value create_value_from(Real value) const;
    Value create_value_from(Vector value) const;
};
