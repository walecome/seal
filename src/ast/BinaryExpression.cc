#include "BinaryExpression.hh"

void BinaryExpression::analyze(Scope *scope) {
    left->analyze(scope);
    op->analyze(scope);
    right->analyze(scope);

    if (left->type == right->type) {
        this->type = left->type;
    } else {
        this->type = Type(Primitive::VOID);
        // @TODO: Determine expected type, left is more likely but not always
        error::mismatched_type(left->type, right->type);
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