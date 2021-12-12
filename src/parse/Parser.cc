#include "Parser.hh"
#include "Error.hh"
#include "ast/CompilationUnit.hh"
#include "ast/StructDecl.hh"
#include "ast/FunctionDecl.hh"

void Parser::parse(TokenBuffer &tokens) {
    ptr_t<CompilationUnit> compilation_unit =
        std::make_unique<CompilationUnit>();

    while (!tokens.empty()) {
        if (tokens.eat(EOF_TOKEN)) break;

        ptr_t<StructDecl> struct_decl = parse_struct(tokens);
        if (struct_decl) {
            // TODO
            ASSERT_NOT_REACHED();
            // compilation_unit->add_struct(struct_decl);
            continue;
        }

        ptr_t<FunctionDecl> function_decl = parse_function_decl(tokens);

        if (function_decl) {
            compilation_unit->add_function(function_decl);
            continue;
        }
        std::cout << tokens.reconstruct_row(tokens.top().row) << std::endl;
        util::print_col_indication(tokens.top().col);
        error::syntax_error("Syntax error, expected struct or function declaration");

    };

    this->compilation_unit = std::move(compilation_unit);
}
