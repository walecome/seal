#include "ast/CalculateExpression.hh"

#include <fmt/format.h>

#include "ast/Operator.hh"

void CalculateExpression::analyze(Scope* scope) {
    BinaryExpression::analyze(scope);
}
