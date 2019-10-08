#include "Scope.hh"
#include "ast/FunctionDecl.hh"
#include "ast/VariableDecl.hh"

void Scope::add_function(const FunctionDecl *const function_decl) {
    if (has_function(function_decl->identifier)) {
        error::add_semantic_error("Redeclaration of function " +
                                  std::string(function_decl->identifier));
        return;
    }
    functions.insert(function_decl->identifier);
}

void Scope::add_variable(const VariableDecl *const variable_decl) {
    if (has_variable(variable_decl->identifier)) {
        error::add_semantic_error("Redeclaration of variable " +
                                  std::string(variable_decl->identifier));
        return;
    }
    variables.insert(variable_decl->identifier);
}

bool Scope::has_function(const std::string_view identifier,
                         bool traverse_parent) const {
    if (functions.find(identifier) != std::end(functions)) return true;
    return traverse_parent && parent && parent->has_function(identifier, true);
}

bool Scope::has_variable(const std::string_view identifier,
                         bool traverse_parent) const {
    if (variables.find(identifier) != std::end(variables)) return true;
    return traverse_parent && parent && parent->has_variable(identifier, true);
}

unsigned Scope::function_count() const { return functions.size(); }