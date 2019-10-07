#include "Scope.hh"
#include "ast/FunctionDecl.hh"
#include "ast/VariableDecl.hh"

void Scope::add_function(const FunctionDecl *const function_decl) {
    if (has_function(function_decl->identifier)) {
        throw std::runtime_error("Redeclaration of function " +
                                 std::string(function_decl->identifier));
    }
    functions.insert(function_decl->identifier);
}

void Scope::add_variable(const VariableDecl *const variable_decl) {
    if (has_variable(variable_decl->identifier)) {
        throw std::runtime_error("Redeclaration of variable " +
                                 std::string(variable_decl->identifier));
    }
    variables.insert(variable_decl->identifier);
}

bool Scope::has_function(const std::string_view identifier) const {
    return functions.find(identifier) != std::end(functions);
}

bool Scope::has_variable(const std::string_view identifier) const {
    return variables.find(identifier) != std::end(variables);
}

unsigned Scope::function_count() const { return functions.size(); }