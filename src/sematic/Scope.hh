#pragma once

#include <string>
#include <unordered_set>

#include "Constants.hh"
#include "ast/FunctionDecl.hh"
#include "ast/VariableDecl.hh"

struct Scope {
    Scope(std::shared_ptr<Scope> parent = nullptr) : parent { parent } {}

    void addFunction(const FunctionDecl *const functionDeclaration);
    void addVariable(const VariableDecl *const variableDeclaration);

    bool hasFunction(const std::string &ident) const;
    bool hasVariable(const std::string &ident) const;

    unsigned functionCount() const;

    std::unordered_set<std::string> variables {};
    std::unordered_set<std::string> functions {};

    std::unordered_set<std::shared_ptr<Scope>> children {};
    std::shared_ptr<Scope> parent;
};