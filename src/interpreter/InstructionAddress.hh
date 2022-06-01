#pragma once

#include <cstddef>

class InstructionAddress {
  public:
    InstructionAddress() = delete;
    explicit InstructionAddress(size_t addr);

    InstructionAddress increment() const;

    size_t unwrap() const;
  private:
};

