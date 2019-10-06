#pragma once

#include <string>
#include <unordered_set>

#include "Constants.hh"

struct FunctionDecl;
struct VariableDecl;

struct Scope {
    Scope(Scope *parent = nullptr) : parent { parent } {}

    void addFunction(const FunctionDecl *const functionDeclaration);
    void addVariable(const VariableDecl *const variableDeclaration);

    bool hasFunction(const std::string_view identifier) const;
    bool hasVariable(const std::string_view identifier) const;

    unsigned functionCount() const;

    std::unordered_set<std::string_view> variables {};
    std::unordered_set<std::string_view> functions {};

    std::shared_ptr<Scope> parent;
};