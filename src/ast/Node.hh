#pragma once

#include <memory>
#include <string>

template <typename T>
using ptr_t = std::unique_ptr<T>;

struct Node {
    virtual ~Node() = default;

    virtual std::string name() const { return "Node"; }

    virtual std::string dump() const { return "Node"; }
};