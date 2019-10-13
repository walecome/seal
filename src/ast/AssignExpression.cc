#include "AssignExpression.hh"
#include "VariableDecl.hh"
#include "VariableExpression.hh"

void AssignExpression::analyze(Scope* scope) {
    auto variable_expr = dynamic_cast<VariableExpression*>(left.get());
    if (!variable_expr) {
        error::add_semantic_error("Left of assign is not variable", source_ref);
    } else {
        auto decl = scope->get_variable(variable_expr->identifier.value);

        if (!decl->is_mutable) {
            error::add_semantic_error("Assign to non-mutable variable",
                                      source_ref);
        }
    }

    BinaryExpression::analyze(scope);
}