#include "Parser.hh"
#include "ast/CompilationUnit.hh"

void Parser::parse(TokenBuffer &tokens) {
    ptr_t<CompilationUnit> compilationUnit =
        std::make_unique<CompilationUnit>();

    while (!tokens.empty()) {
        ptr_t<Function> function = parseFunction(tokens);

        if (!function) throw std::runtime_error("Unable to parse function");
        compilationUnit->addFunction(function);
    };

    this->compilationUnit = std::move(compilationUnit);
}