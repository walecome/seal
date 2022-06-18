#pragma once

#include <cstddef>
#include <string>

class InstructionAddress {
  public:
    InstructionAddress();
    explicit InstructionAddress(size_t addr);

    InstructionAddress increment() const;

    size_t unwrap() const;

    std::string as_hex() const;
  private:

    size_t m_addr;
};

