#include <memory>
#include "Parser.hh"
#include "ast/FunctionDecl.hh"
#include "ast/FunctionDeclC.hh"
#include "ast/FunctionDeclUser.hh"
#include "ast/ParameterList.hh"
#include "ast/VariableDecl.hh"

ptr_t<FunctionDecl> Parser::parse_function_decl(TokenBuffer &tokens) {
    auto begin = tokens.top_iterator();

    if (!tokens.eat(FUNC_KEYWORD)) {
        return nullptr;
    }

    Token identifier = tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parse_parameter_list(tokens);

    tokens.expect(ARROW);

    Type type = parse_type(tokens);

    if (tokens.can_pop(TokenType::IN_KEYWORD)) {
        return parse_c_function_decl(tokens, begin, identifier.value, parameters, type);
    } else {
        return parse_user_function_decl(tokens, begin, identifier.value, parameters, type);
    }
}

ptr_t<FunctionDeclC> Parser::parse_c_function_decl(
    TokenBuffer &tokens, TokenBuffer::Iterator begin, std::string_view identifier,
    ptr_t<ParameterList> &parameters, Type type) {

    tokens.eat(TokenType::IN_KEYWORD);
    if (!tokens.can_pop(TokenType::STRING)) {
        return nullptr;
    }
    auto lib_name_quoted = tokens.top().value;
    std::string_view lib_name = lib_name_quoted;
    lib_name.remove_prefix(1);
    lib_name.remove_suffix(1);
    tokens.expect(TokenType::STRING);
    
    auto end = tokens.top_iterator();
    auto function_decl = std::make_unique<FunctionDeclC>(identifier, parameters, type, lib_name);

    function_decl->source_ref.begin = begin;
    function_decl->source_ref.end = end;

    return function_decl;
    
}

ptr_t<FunctionDeclUser> Parser::parse_user_function_decl(
    TokenBuffer &tokens, TokenBuffer::Iterator begin, std::string_view identifier,
    ptr_t<ParameterList> &parameters, Type type) {

    ptr_t<Block> body = parse_block(tokens);
    // @TODO: Probably throw an error here
    if (!body) {
        return nullptr;
    }

    auto end = tokens.top_iterator();
    auto function_decl =
        std::make_unique<FunctionDeclUser>(identifier, parameters, type, body);

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
    Token token = tokens.top();
    if (token.type != IDENTIFIER) return nullptr;
    tokens.eat(IDENTIFIER);

    tokens.expect(COLON);

    Type type = parse_type(tokens);

    auto end = tokens.top_iterator();
    auto parameter = std::make_unique<VariableDecl>(token.value, type);

    parameter->source_ref.begin = begin;
    parameter->source_ref.end = end;

    return parameter;
}
