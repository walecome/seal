#include "ast/LogicalExpression.hh"

void LogicalExpression::analyze(Scope *scope) {
    BinaryExpression::analyze(scope);

    if (!m_left->type().is_bool()) {
        error::add_semantic_error(
            "Left hand side of logical expression is not bool", source_ref);
    }

    if (!m_right->type().is_bool()) {
        error::add_semantic_error(
            "Right hand side of logical expression is not bool", source_ref);
    }
}