#include "FunctionDecl.hh"

void FunctionDecl::analyze(Scope *scope) {
    // @TODO: We don't check the function identifier since this is done in the
    //        initial sematic pass. But this creates an issue with not allowing
    //        functions inside other functions.
    ptr_t<Scope> inner_scope = std::make_unique<Scope>(scope, this);
    parameter_list->analyze(inner_scope.get());
    body->analyze(inner_scope.get());
}

std::string FunctionDecl::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << "(" << std::endl;
    oss << util::indent(indent + 1) << "\"" << identifier.value << "\""
        << std::endl;
    if (parameter_list)
        oss << parameter_list->dump(indent + 1) << std::endl;
    else
        oss << util::indent(indent + 1) << "EMPTY PARAMETERLIST" << std::endl;
    oss << util::indent(indent + 1) << type.dump(0) << std::endl;
    oss << body->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";

    return oss.str();
}