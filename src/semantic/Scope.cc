#include "Scope.hh"
#include "ast/FunctionDecl.hh"
#include "ast/VariableDecl.hh"

void Scope::add_function(FunctionDecl *const function_decl) {
    if (has_function(function_decl->identifier().value, false)) {
        error::add_semantic_error("Redeclaration of function",
                                  function_decl->source_ref);
        return;
    }
    functions.insert({ function_decl->identifier().value, function_decl });
}

void Scope::add_variable(VariableDecl *const variable_decl) {
    if (has_variable(variable_decl->identifier().value, false)) {
        error::add_semantic_error("Redeclaration of variable",
                                  variable_decl->source_ref);
        return;
    }
    variables.insert({ variable_decl->identifier().value, variable_decl });
}

bool Scope::has_function(const std::string_view identifier,
                         bool traverse_parent) const {
    if (functions.find(identifier) != std::end(functions)) return true;
    if (traverse_parent && parent) {
        return parent->has_function(identifier, true);
    }
    return false;
}

bool Scope::has_variable(const std::string_view identifier,
                         bool traverse_parent) const {
    if (variables.find(identifier) != std::end(variables)) return true;
    if (traverse_parent && parent) {
        return parent->has_variable(identifier, true);
    }
    return false;
}

unsigned Scope::function_count() const { return functions.size(); }
bool Scope::is_top_level() const { return parent == nullptr; }

FunctionDecl *Scope::get_function(const std::string_view identifier) const {
    auto it = functions.find(identifier);
    if (it != std::end(functions)) return it->second;

    if (parent) return parent->get_function(identifier);

    // If function is not found
    ASSERT_NOT_REACHED();
    return nullptr;
}
VariableDecl *Scope::get_variable(const std::string_view identifier) const {
    auto it = variables.find(identifier);
    if (it != std::end(variables)) return it->second;

    if (parent) return parent->get_variable(identifier);

    // If function is not found
    ASSERT_NOT_REACHED();
    return nullptr;
}
