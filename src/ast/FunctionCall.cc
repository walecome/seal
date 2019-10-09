#include "FunctionCall.hh"
#include "FunctionDecl.hh"

void FunctionCall::analyze(Scope *scope) {
    if (!scope->has_function(identifier.value, true)) {
        error::add_semantic_error("Call to undeclared function", identifier);
        type = Type { Primitive::VOID };
        return;
    }

    auto decl = scope->get_function(identifier.value);

    type = decl->type;

    if (decl->parameter_list->parameters.size() !=
        argument_list->arguments.size()) {
        error::add_semantic_error(
            "Function call argument list length does not match declaration",
            identifier);
        return;
    }

    // Resolves the type for the arguments
    argument_list->analyze(scope);

    for (unsigned i = 0; i < argument_list->arguments.size(); ++i) {
        const auto &arg = argument_list->arguments.at(i);
        const auto &param = decl->parameter_list->parameters.at(i);

        if (arg->type != param->type) {
            error::mismatched_type(arg->type, param->type, source_ref);
        }
    }
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