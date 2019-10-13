#include "Parser.hh"
#include "ast/CompilationUnit.hh"

void Parser::parse(TokenBuffer &tokens) {
    ptr_t<CompilationUnit> compilation_unit =
        std::make_unique<CompilationUnit>();

    while (!tokens.empty()) {
        ptr_t<FunctionDecl> function_decl = parse_function_decl(tokens);

        if (!function_decl)
            throw std::runtime_error("Unable to parse function declaration");
        compilation_unit->add_function(function_decl);
    };

    this->compilation_unit = std::move(compilation_unit);
}