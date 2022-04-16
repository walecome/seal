#include "ast/CalculateExpression.hh"

#include <fmt/format.h>

#include "ast/AstVisitor.hh"
#include "ast/Operator.hh"

void CalculateExpression::analyze(Scope* scope) {
    BinaryExpression::analyze(scope);
}

void CalculateExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}
