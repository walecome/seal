#include "Parser.hh"
#include "ast/FunctionDecl.hh"
#include "ast/ParameterList.hh"
#include "ast/VariableDecl.hh"

ptr_t<FunctionDecl> Parser::parse_function_decl(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(FUNC_KEYWORD)) {
        return nullptr;
    }

    Token identifier = tokens.top();
    tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parse_parameter_list(tokens);

    tokens.expect(ARROW);

    Type type = parse_type(tokens);

    ptr_t<Block> body = parse_block(tokens);
    // @TODO: Probably throw an error here
    if (!body) return nullptr;

    auto end = tokens.top_iterator();
    auto function_decl =
        std::make_unique<FunctionDecl>(identifier, parameters, body, type);

    function_decl->source_ref.begin = begin;
    function_decl->source_ref.end = end;

    return function_decl;
}

ptr_t<ParameterList> Parser::parse_parameter_list(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    tokens.expect(LPARENS);
    ptr_t<ParameterList> parameters = std::make_unique<ParameterList>();
    if (tokens.eat(RPARENS)) return parameters;

    while (ptr_t<VariableDecl> parameter = parse_parameter(tokens)) {
        parameters->add_parameter(parameter);
        if (!tokens.eat(COMMA)) break;
    }

    tokens.expect(RPARENS);

    auto end = tokens.top_iterator();

    parameters->source_ref.begin = begin;
    parameters->source_ref.end = end;

    return parameters;
}

ptr_t<VariableDecl> Parser::parse_parameter(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();
    Token identifier = tokens.top();
    if (identifier.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    tokens.expect(COLON);

    Type type = parse_type(tokens);

    auto end = tokens.top_iterator();
    auto parameter = std::make_unique<VariableDecl>(identifier, type);

    parameter->source_ref.begin = begin;
    parameter->source_ref.end = end;

    return parameter;
}
