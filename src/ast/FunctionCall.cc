#include "FunctionCall.hh"
#include "FunctionDecl.hh"

#include "builtin/BuiltIn.hh"

void FunctionCall::analyze(Scope *scope) {
    // Resolves the type for the arguments
    m_argument_list->analyze(scope);

    if (BuiltIn::is_builtin(identifier())) {
        if (BuiltIn::is_typechecked(identifier())) {
            // @TODO: Handle typechecking for built in
        }
        m_type = Type { Primitive::DONT_CARE };
        return;
    }

    if (!scope->has_function(identifier(), true)) {
        error::add_semantic_error("Call to undeclared function", source_ref);
        m_type = Type { Primitive::VOID };
        return;
    }

    if (!m_decl) {
        auto decl = scope->get_function(identifier());
        m_decl = decl;
        m_type = decl->type();
    }

    if (m_decl->parameter_list()->nr_params() != m_argument_list->nr_args()) {
        error::add_semantic_error(
            "Function call argument list length does not match declaration",
            source_ref);
        return;
    }

    for (unsigned i = 0; i < m_argument_list->nr_args(); ++i) {
        const auto &arg = m_argument_list->argument_at(i);
        const auto &param = m_decl->parameter_list()->parameter_at(i);

        if (arg->type() != param->type()) {
            error::mismatched_type(arg->type(), param->type(), source_ref);
        }
    }
}

std::string FunctionCall::dump(unsigned indent) const {
    std::ostringstream oss {};

    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << identifier() << "\""
        << std::endl;
    oss << m_argument_list->dump(indent + 1);

    oss << util::indent(indent) << ")";

    return oss.str();
}
