#pragma once

#include <cassert>
#include <string>
#include <unordered_map>

#include "Constants.hh"
#include "Error.hh"

struct FunctionDecl;
struct VariableDecl;

struct Scope {
    Scope(Scope *parent = nullptr) : parent { parent } {}

    void add_function(FunctionDecl *const function_declaration);
    void add_variable(VariableDecl *const variable_declaration);

    bool has_function(const std::string_view identifier,
                      bool traverse_parent = false) const;
    bool has_variable(const std::string_view identifier,
                      bool traverse_parent = false) const;

    FunctionDecl *get_function(const std::string_view identifier) const;
    VariableDecl *get_variable(const std::string_view identifier) const;

    unsigned function_count() const;

    std::unordered_map<std::string_view, VariableDecl *const> variables {};
    std::unordered_map<std::string_view, FunctionDecl *const> functions {};

    Scope *const parent;
};