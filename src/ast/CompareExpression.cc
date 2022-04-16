#include "ast/CompareExpression.hh"

#include "ast/AstVisitor.hh"

void CompareExpression::accept(const AstVisitor &visitor) {
    visitor.visit(*this);
}
