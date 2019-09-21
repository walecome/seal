#pragma once

#include <string>

struct Node {
    virtual ~Node() = default;

    virtual std::string name() const { return "Node"; }
};