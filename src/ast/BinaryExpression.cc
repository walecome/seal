#include "BinaryExpression.hh"
#include "Operator.hh"
void BinaryExpression::analyze(Scope *scope) {
    left->analyze(scope);
    op->analyze(scope);
    right->analyze(scope);

    if (left->type == right->type) {
        // @REFACTOR: Ugly logic
        if (this->type.primitive != Primitive::VOID) return;
        this->type = left->type;
    } else {
        this->type = Type(Primitive::VOID);
        error::mismatched_type(left->type, right->type, source_ref);
    }
}

std::string BinaryExpression::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << left->dump(indent + 1) << std::endl;
    oss << op->dump(indent + 1) << std::endl;
    oss << right->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}