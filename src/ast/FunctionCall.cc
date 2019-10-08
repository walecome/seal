#include "FunctionCall.hh"

void FunctionCall::analyze(Scope *scope) {
    if (!scope->has_function(identifier.value, true)) {
        // @TODO: Implement semantic error instead
        error::add_semantic_error("Call to undeclared function", identifier);
    }

    argument_list->analyze(scope);
}

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << identifier.value << "\""
        << std::endl;
    oss << argument_list->dump(indent + 1);

    oss << util::indent(indent) << ")";

    return oss.str();
}