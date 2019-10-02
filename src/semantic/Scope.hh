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

    bool hasFunction(const std::string &ident) const;
    bool hasVariable(const std::string &ident) const;

    unsigned functionCount() const;

    std::unordered_set<std::string> variables {};
    std::unordered_set<std::string> functions {};

    std::shared_ptr<Scope> parent;
};