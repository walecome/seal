#include "FunctionCall.hh"
#include "FunctionDecl.hh"

#include "builtin/BuiltIn.hh"

void FunctionCall::analyze(Scope *scope) {
    if (BuiltIn::is_builtin(m_identifier.value)) {
        if (BuiltIn::is_typechecked(m_identifier.value)) {
            // @TODO: Handle typechecking for built in
        }
        return;
    }

    if (!scope->has_function(m_identifier.value, true)) {
        throw 1;
        error::add_semantic_error("Call to undeclared function", source_ref);
        m_type = Type { Primitive::VOID };
        return;
    }

    auto decl = scope->get_function(m_identifier.value);

    m_decl = decl;
    m_type = decl->type();

    if (decl->parameter_list()->nr_params() != m_argument_list->nr_args()) {
        error::add_semantic_error(
            "Function call argument list length does not match declaration",
            source_ref);
        return;
    }

    // Resolves the type for the arguments
    m_argument_list->analyze(scope);

    for (unsigned i = 0; i < m_argument_list->nr_args(); ++i) {
        const auto &arg = m_argument_list->argument_at(i);
        const auto &param = decl->parameter_list()->parameter_at(i);

        if (arg->type() != param->type()) {
            error::mismatched_type(arg->type(), param->type(), source_ref);
        }
    }
}

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << m_identifier.value << "\""
        << std::endl;
    oss << m_argument_list->dump(indent + 1);

    oss << util::indent(indent) << ")";

    return oss.str();
}