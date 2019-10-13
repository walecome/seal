#include "ast/Expression.hh"
#include "Parser.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/BooleanLiteral.hh"
#include "ast/CompareExpression.hh"
#include "ast/EqualityExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Operator.hh"
#include "ast/StringLiteral.hh"
#include "ast/UnaryExpression.hh"
#include "ast/VariableExpression.hh"

ptr_t<Expression> Parser::parse_expression(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();
    auto expression = parse_assign(tokens);
    auto end = tokens.top_iterator();

    expression->source_ref.begin = begin;
    expression->source_ref.end = end;
    return expression;
}

ptr_t<Expression> Parser::parse_assign(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();
    ptr_t<Expression> first = parse_equality(tokens);

    while (tokens.eat(ASSIGN)) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_equality(tokens);
        first = std::make_unique<AssignExpression>(first, op, second);
    }

    auto end = tokens.top_iterator();
    first->source_ref.begin = begin;
    first->source_ref.end = end;

    return first;
}

ptr_t<Expression> Parser::parse_equality(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();

    ptr_t<Expression> first = parse_compare(tokens);

    while (tokens.eat({ EQ, NOT_EQ })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_compare(tokens);
        first = std::make_unique<EqualityExpression>(first, op, second);
    }

    auto end = tokens.top_iterator();
    first->source_ref.begin = begin;
    first->source_ref.end = end;

    return first;
}

ptr_t<Expression> Parser::parse_compare(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();
    ptr_t<Expression> first = parse_add_sub(tokens);

    while (tokens.eat({ GT, GTEQ, LT, LTEQ })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_add_sub(tokens);
        first = std::make_unique<CompareExpression>(first, op, second);
    }

    auto end = tokens.top_iterator();

    first->source_ref.begin = begin;
    first->source_ref.end = end;

    return first;
}

ptr_t<Expression> Parser::parse_add_sub(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();
    ptr_t<Expression> first = parse_mult_div(tokens);

    while (tokens.eat({ PLUS, MINUS })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_mult_div(tokens);
        first = std::make_unique<BinaryExpression>(first, op, second);
    }

    auto end = tokens.top_iterator();
    first->source_ref.begin = begin;
    first->source_ref.end = end;

    return first;
}

ptr_t<Expression> Parser::parse_mult_div(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();
    ptr_t<Expression> first = parse_unary(tokens);

    while (tokens.eat({ STAR, SLASH })) {
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.previous());
        ptr_t<Expression> second = parse_unary(tokens);
        first = std::make_unique<BinaryExpression>(first, op, second);
    }

    auto end = tokens.top_iterator();
    first->source_ref.begin = begin;
    first->source_ref.end = end;

    return first;
}

ptr_t<Expression> Parser::parse_unary(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();

    ptr_t<Expression> expression;

    if (tokens.top().type == MINUS) {
        // @REFACTOR: We should probably have like parse_operator
        // instead...
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.pop());
        ptr_t<Expression> right = parse_unary(tokens);
        expression = std::make_unique<UnaryExpression>(op, right);
    } else {
        expression = Parser::parse_primary(tokens);
    }

    auto end = tokens.top_iterator();

    expression->source_ref.begin = begin;
    expression->source_ref.end = end;

    return expression;
}

ptr_t<Expression> Parser::parse_primary(TokenBuffer& tokens) {
    ptr_t<Expression> primary;

    auto begin = tokens.top_iterator();

    Token current = tokens.pop();

    if (current.type == NUMBER) {
        int value = std::stoi(std::string(current.value));
        primary = std::make_unique<IntegerLiteral>(value);
    } else if (current.type == STRING) {
        std::string value = std::string(current.value);
        primary = std::make_unique<StringLiteral>(value);
    } else if (current.type == LPARENS) {
        primary = parse_expression(tokens);
        tokens.expect(RPARENS);
    } else if (current.type == BOOL) {
        bool value = (current.value == "true");
        primary = std::make_unique<BooleanLiteral>(value);
    } else if (current.type == IDENTIFIER) {
        if (!(primary = parse_function_call(tokens))) {
            primary = std::make_unique<VariableExpression>(current);
        }
    } else {
        // @TODO: Throw parse error
        throw std::runtime_error("Unexpected primary expression" +
                                 current.to_string());
    }

    auto end = tokens.top_iterator();
    primary->source_ref.begin = begin;
    primary->source_ref.end = end;

    return primary;
}

ptr_t<FunctionCall> Parser::parse_function_call(TokenBuffer& tokens) {
    // Need the identifier aswell
    auto begin = tokens.top_iterator() - 1;

    Token identifier = tokens.previous();
    // Identifier already eaten
    if (tokens.top().type != LPARENS) return nullptr;

    ptr_t<ArgumentList> argument_list = parse_argument_list(tokens);

    auto end = tokens.top_iterator();

    auto function_call =
        std::make_unique<FunctionCall>(identifier, argument_list);

    function_call->source_ref.begin = begin;
    function_call->source_ref.end = end;

    return function_call;
}