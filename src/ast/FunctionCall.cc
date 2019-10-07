#include "FunctionCall.hh"

void FunctionCall::analyze(Scope *scope) const {
    if (!scope->has_function(identifier)) {
        // @TODO: Implement semantic error instead
        throw std::runtime_error("Call to undeclared function " +
                                 std::string(identifier));
    }

    argument_list->analyze(scope);
}

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << identifier << "\"" << std::endl;
    oss << argument_list->dump(indent + 1);

    oss << util::indent(indent) << ")";

    return oss.str();
}