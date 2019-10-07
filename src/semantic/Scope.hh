#pragma once

#include <string>
#include <unordered_set>

#include "Constants.hh"

struct FunctionDecl;
struct VariableDecl;

struct Scope {
    Scope(Scope *parent = nullptr) : parent { parent } {}

    void add_function(const FunctionDecl *const function_declaration);
    void add_variable(const VariableDecl *const variable_declaration);

    bool has_function(const std::string_view identifier) const;
    bool has_variable(const std::string_view identifier) const;

    unsigned function_count() const;

    std::unordered_set<std::string_view> variables {};
    std::unordered_set<std::string_view> functions {};

    std::shared_ptr<Scope> parent;
};