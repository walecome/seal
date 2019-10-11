#include "ast/Expression.hh"
#include "Parser.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Operator.hh"
#include "ast/StringLiteral.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableExpression.hh"

ptr_t<Expression> Parser::parse_expression(TokenBuffer& tokens) {
    return parse_assign(tokens);
}

ptr_t<Expression> Parser::parse_assign(TokenBuffer& tokens) {
    ptr_t<Expression> first = parse_compare(tokens);

    while (tokens.eat(ASSIGN)) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_compare(tokens);
        first = std::make_unique<AssignExpression>(first, op, second);
    }

    return first;
}

ptr_t<Expression> Parser::parse_compare(TokenBuffer& tokens) {
    ptr_t<Expression> first = parse_add_sub(tokens);

    while (tokens.eat({ GT, GTEQ, LT, LTEQ, EQ, NOT_EQ })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_add_sub(tokens);
        first = std::make_unique<BinaryExpression>(first, op, second);
    }

    return first;
}

ptr_t<Expression> Parser::parse_add_sub(TokenBuffer& tokens) {
    ptr_t<Expression> first = parse_mult_div(tokens);

    while (tokens.eat({ PLUS, MINUS })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_mult_div(tokens);
        first = std::make_unique<BinaryExpression>(first, op, second);
    }

    return first;
}

ptr_t<Expression> Parser::parse_mult_div(TokenBuffer& tokens) {
    ptr_t<Expression> first = parse_unary(tokens);

    while (tokens.eat({ STAR, SLASH })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_unary(tokens);
        first = std::make_unique<BinaryExpression>(first, op, second);
    }

    return first;
}

ptr_t<Expression> Parser::parse_unary(TokenBuffer& tokens) {
    if (tokens.top().type == MINUS) {
        // @REFACTOR: We should probably have like parse_operator
        // instead...
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.pop());
        ptr_t<Expression> right = parse_unary(tokens);
        return std::make_unique<UnaryExpression>(op, right);
    }

    return Parser::parse_primary(tokens);
}

ptr_t<Expression> Parser::parse_primary(TokenBuffer& tokens) {
    ptr_t<Expression> primary;

    Token current = tokens.pop();

    if (current.type == NUMBER) {
        primary = std::make_unique<IntegerLiteral>(current);
    } else if (current.type == STRING) {
        primary = std::make_unique<StringLiteral>(current);
    } else if (current.type == LPARENS) {
        primary = parse_expression(tokens);
        tokens.expect(RPARENS);
    } else if (current.type == IDENTIFIER) {
        if (!(primary = parse_function_call(tokens))) {
            primary = std::make_unique<VariableExpression>(current);
        }
    } else {
        // @TODO: Throw parse error
        throw 1;
    }

    return primary;
}

ptr_t<FunctionCall> Parser::parse_function_call(TokenBuffer& tokens) {
    Token identifier = tokens.previous();
    // Identifier already eaten
    if (tokens.top().type != LPARENS) return nullptr;

    ptr_t<ArgumentList> argument_list = parse_argument_list(tokens);

    return std::make_unique<FunctionCall>(identifier, argument_list);
}