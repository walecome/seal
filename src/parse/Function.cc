#include "ast/Function.hh"
#include "Parser.hh"
#include "ast/FunctionDecl.hh"
#include "ast/ParameterList.hh"
#include "ast/VariableDecl.hh"

ptr_t<Function> Parser::parse_function(TokenBuffer &tokens) {
    ptr_t<FunctionDecl> function_decl_node = parse_function_decl(tokens);
    if (!function_decl_node) return nullptr;

    ptr_t<Block> block_node = parse_block(tokens);
    if (!block_node) return nullptr;

    return std::make_unique<Function>(function_decl_node, block_node);
}

ptr_t<FunctionDecl> Parser::parse_function_decl(TokenBuffer &tokens) {
    if (!tokens.eat(FUNC_KEYWORD)) {
        return nullptr;
    }

    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parse_parameter_list(tokens);

    tokens.expect(ARROW);
    tokens.expect(TYPE);

    return std::make_unique<FunctionDecl>(identifier.value, parameters);
}

ptr_t<ParameterList> Parser::parse_parameter_list(TokenBuffer &tokens) {
    tokens.expect(LPARENS);
    ptr_t<ParameterList> parameters = std::make_unique<ParameterList>();
    if (tokens.eat(RPARENS)) return parameters;

    while (ptr_t<VariableDecl> parameter = parse_parameter(tokens)) {
        parameters->add_parameter(parameter);
        if (!tokens.eat(COMMA)) break;
    }

    tokens.expect(RPARENS);

    return parameters;
}

ptr_t<VariableDecl> Parser::parse_parameter(TokenBuffer &tokens) {
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    tokens.expect(COLON);

    Token type = tokens.top();
    tokens.expect(TYPE);

    return std::make_unique<VariableDecl>(identifier.value, type.value);
}
