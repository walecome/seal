#include "Scope.hh"
#include "ast/FunctionDecl.hh"
#include "ast/VariableDecl.hh"

void Scope::addFunction(const FunctionDecl *const functionDeclaration) {
    if (hasFunction(functionDeclaration->identifier)) {
        throw std::runtime_error("Redeclaration of function " +
                                 functionDeclaration->identifier);
    }
    functions.insert(functionDeclaration->identifier);
}

void Scope::addVariable(const VariableDecl *const variableDeclaration) {
    if (hasVariable(variableDeclaration->identifier)) {
        throw std::runtime_error("Redeclaration of variable " +
                                 variableDeclaration->identifier);
    }
    variables.insert(variableDeclaration->identifier);
}

bool Scope::hasFunction(const std::string &ident) const {
    return functions.find(ident) != std::end(functions);
}

bool Scope::hasVariable(const std::string &ident) const {
    return variables.find(ident) != std::end(variables);
}

unsigned Scope::functionCount() const { return functions.size(); }