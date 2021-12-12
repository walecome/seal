#include "parse/Parser.hh"

#include "ast/MemberFunctionDecl.hh"
#include "ast/StructDecl.hh"
#include "ast/VariableDecl.hh"

#include "base/Logging.hh"

namespace {
    using base::log;
}

ptr_t<StructDecl> Parser::parse_struct(TokenBuffer &tokens) {
    SourceRefGuard source_ref_guard(tokens);

    if (!tokens.eat(TokenType::STRUCT)) {
        return nullptr;
    }

    Token identifier = tokens.expect(IDENTIFIER);

    tokens.expect(LBRACE);

    std::vector<ptr_t<VariableDecl>> members;
    std::vector<ptr_t<MemberFunctionDecl>> member_functions;

    auto get_member = [this, &members, &tokens] () -> bool {
        auto variable_decl = parse_variable_decl(tokens);
        if (variable_decl) {
            members.push_back(std::move(variable_decl));
            return true;
        }

        return false;
    };

    auto get_member_function = [this, &member_functions, &tokens] () -> bool {
        auto member_function_decl = parse_member_function(tokens);
        if (member_function_decl) {
            member_functions.push_back(std::move(member_function_decl));
            return true;
        }

        return false;
    };

    while (!tokens.eat(RBRACE)) {
        if (get_member()) continue;
        if (get_member_function()) continue;
        error::syntax_error(
            fmt::format("Syntax error: Excepted member or member functiond declaration, found token \"{}\"",
                        tokens.top().value));
        return nullptr;
    }

    auto struct_decl = std::make_unique<StructDecl>(
        identifier.value,
        members,
        member_functions
    );

    source_ref_guard.finalize(struct_decl.get());
    return struct_decl;
}

ptr_t<MemberFunctionDecl> Parser::parse_member_function(TokenBuffer &tokens) {
    SourceRefGuard source_ref_guard(tokens);

    if (!tokens.eat(FUNC_KEYWORD)) {
        return nullptr;
    }

    Token identifier = tokens.expect(IDENTIFIER);

    ptr_t<ParameterList> parameters = parse_parameter_list(tokens);
    tokens.expect(ARROW);

    Type type = parse_type(tokens);
    tokens.expect(SEMICOLON);

    auto member_function_decl = std::make_unique<MemberFunctionDecl>(
        identifier.value,
        parameters,
        type
    );
    source_ref_guard.finalize(member_function_decl.get());
    return member_function_decl;
}
