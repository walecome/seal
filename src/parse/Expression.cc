#include "ast/Expression.hh"
#include "Parser.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/BinaryExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Operator.hh"
#include "ast/StringLiteral.hh"
#include "ast/VariableExpression.hh"

ptr_t<UnaryExpression> Parser::parse_unary(TokenBuffer& tokens) {
    // @TODO: Add source_ref
    ptr_t<UnaryExpression> unary_expression;

    if (tokens.top().type == MINUS) {
        // @REFACTOR: We should probably have like parse_operator instead...
        ptr_t<Operator> op = std::make_unique<Operator>(tokens.pop());
        unary_expression =
            std::make_unique<UnaryExpression>(op, Parser::parse_unary(tokens));
    } else {
        unary_expression = Parser::parse_unary(tokens);
    }

    return unary_expression;
}

ptr_t<Expression> Parser::parse_primary(TokenBuffer& tokens) {
    // @TODO: Add source_ref
    ptr_t<Expression> primary;

    Token current = tokens.pop();

    if (current.type == NUMBER) {
        primary = std::make_unique<IntegerLiteral>(current);
    } else if (current.type == STRING) {
        primary = std::make_unique<StringLiteral>(current);
    } else if (current.type == LPARENS) {
        primary = parse_expression(tokens);
        tokens.expect(RPARENS);
    } else {
        // @TODO: Throw parse error
        throw 1;
    }

    return primary;
}

ptr_t<Expression> Parser::parse_expression(TokenBuffer& tokens) {}
