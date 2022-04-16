#include "ast/EqualityExpression.hh"

#include "ast/AstVisitor.hh"

void EqualityExpression::accept(const AstVisitor& visitor) {
    visitor.visit(*this);
}
