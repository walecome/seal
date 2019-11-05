#include "VariableDecl.hh"

void VariableDecl::analyze(Scope *scope) {
    scope->add_variable(this);

    // Parameters don't have values
    if (!value) return;

    // Analyzing the value expression will resolve its type
    value->analyze(scope);

    if (value->type.primitive == Primitive::DONT_CARE)
        value->type.primitive = type.primitive;

    if (type != value->type) {
        error::mismatched_type(type, value->type, source_ref);
    }
}

std::string VariableDecl::dump(unsigned indent) const {
    std::ostringstream oss {};
    oss << util::indent(indent) << name() << " (" << std::endl;
    oss << util::indent(indent + 1) << "\"" << identifier.value << "\" : ";
    oss << type.dump(0) << std::endl;

    if (value) oss << value->dump(indent + 1) << std::endl;
    oss << util::indent(indent) << ")";
    return oss.str();
}