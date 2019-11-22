#include "ast/Expression.hh"
#include "Parser.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/AssignExpression.hh"
#include "ast/BinaryExpression.hh"
#include "ast/BooleanLiteral.hh"
#include "ast/CompareExpression.hh"
#include "ast/EqualityExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/IndexExpression.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Operator.hh"
#include "ast/RealLiteral.hh"
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

bool is_index(Token token) { return token.value == "["; }

ptr_t<IndexExpression> Parser::parse_index_expression(
    TokenBuffer& tokens, ptr_t<Expression>& indexed) {
    tokens.expect(LBRACKET);
    if (tokens.eat(RBRACKET)) {
        // @TODO: Throw parse error
        throw 1;
    }

    ptr_t<Expression> index = parse_expression(tokens);
    tokens.expect(RBRACKET);

    return std::make_unique<IndexExpression>(indexed, index);
}

bool is_valid_prefix(TokenType token_type) {
    switch (token_type) {
        case MINUS:
        case INC:
        case DEC:
            return true;

        default:
            return false;
    }
}

ptr_t<Expression> Parser::parse_unary(TokenBuffer& tokens) {
    auto begin = tokens.top_iterator();

    ptr_t<Expression> expression;

    if (is_valid_prefix(tokens.top().type)) {
        // @REFACTOR: We should probably have like parse_operator
        // instead...
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.pop());
        ptr_t<Expression> right = parse_unary(tokens);
        expression = std::make_unique<UnaryExpression>(op, right);
    } else {
        expression = Parser::parse_primary(tokens);

        if (is_index(tokens.top())) {
            expression = parse_index_expression(tokens, expression);
        }
    }

    auto end = tokens.top_iterator();

    expression->source_ref.begin = begin;
    expression->source_ref.end = end;

    return expression;
}

ptr_t<Expression> parse_number(TokenBuffer& tokens) {
    Token first = tokens.previous();

    std::ostringstream oss {};
    oss << first.value;

    if (tokens.eat(DOT)) {
        oss << "." << tokens.pop().value;
        return std::make_unique<RealLiteral>(std::stod(oss.str()));
    }

    return std::make_unique<IntegerLiteral>(std::stoi(oss.str()));
}

ptr_t<Expression> Parser::parse_primary(TokenBuffer& tokens) {
    ptr_t<Expression> primary;

    auto begin = tokens.top_iterator();

    Token current = tokens.pop();

    if (current.type == NUMBER) {
        primary = parse_number(tokens);
    } else if (current.type == STRING) {
        std::string_view quotes_removed = current.value;
        quotes_removed.remove_prefix(1);
        quotes_removed.remove_suffix(1);

        primary = std::make_unique<StringLiteral>(quotes_removed);
    } else if (current.type == LPARENS) {
        primary = parse_expression(tokens);
        tokens.expect(RPARENS);
    } else if (current.type == BOOL) {
        bool value = (current.value == "true");
        primary = std::make_unique<BooleanLiteral>(value);
    } else if (current.type == IDENTIFIER) {
        if (!(primary = parse_function_call(tokens))) {
            primary = std::make_unique<VariableExpression>(current.value);
        }
    } else if (current.type == LBRACKET) {
        primary = parse_array_literal(tokens);
    } else {
        // @TODO: Improve this error
        tokens.dump_current_row("Error parsing expression, expected primary");
        // @FIXME: This should not be here
        exit(EXIT_FAILURE);
    }

    auto end = tokens.top_iterator();
    primary->source_ref.begin = begin;
    primary->source_ref.end = end;

    return primary;
}

ptr_t<FunctionCall> Parser::parse_function_call(TokenBuffer& tokens) {
    // Need the identifier aswell
    auto begin = tokens.top_iterator() - 1;

    std::string_view identifier = tokens.previous().value;

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