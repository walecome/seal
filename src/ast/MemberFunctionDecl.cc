#include "ast/MemberFunctionDecl.hh"


std::string MemberFunctionDecl::dump(unsigned int indent) const {
    return FunctionDecl::dump(indent);
}
