#include "AssignExpression.hh"
#include "IndexExpression.hh"
#include "VariableDecl.hh"
#include "VariableExpression.hh"

void AssignExpression::analyze(Scope* scope) {
    auto variable_expr = dynamic_cast<VariableExpression*>(m_left.get());
    auto index_expr = dynamic_cast<IndexExpression*>(m_left.get());

    bool check_decl = true;

    if (variable_expr) {
        ;
    } else if (index_expr) {
        variable_expr =
            dynamic_cast<VariableExpression*>(index_expr->indexed_expression());

    } else {
        error::add_semantic_error("Left of assign is not variable", source_ref);

        check_decl = false;
    }

    if (check_decl) {
        auto decl = scope->get_variable(variable_expr->identifier());

        if (!decl->is_mutable()) {
            error::add_semantic_error("Assign to non-mutable variable",
                                      source_ref);
        }
    }

    BinaryExpression::analyze(scope);
}