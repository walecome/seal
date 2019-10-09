#include "ReturnStatement.hh"
#include "FunctionDecl.hh"

void ReturnStatement::analyze(Scope *scope) {
    return_value->analyze(scope);

    if (return_value->type != scope->context->type) {
        error::mismatched_type(scope->context->type, return_value->type,
                               source_ref);
    }
}

std::string ReturnStatement::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << return_value->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}