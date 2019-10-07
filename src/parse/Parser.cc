#include "Parser.hh"
#include "ast/CompilationUnit.hh"

void Parser::parse(TokenBuffer &tokens) {
    ptr_t<CompilationUnit> compilation_unit =
        std::make_unique<CompilationUnit>();

    while (!tokens.empty()) {
        ptr_t<Function> function = parse_function(tokens);

        if (!function) throw std::runtime_error("Unable to parse function");
        compilation_unit->add_function(function);
    };

    this->compilation_unit = std::move(compilation_unit);
}