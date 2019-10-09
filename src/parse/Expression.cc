#include "ast/Expression.hh"
#include "Parser.hh"
#include "ast/ArrayLiteral.hh"
#include "ast/BinaryExpression.hh"
#include "ast/FunctionCall.hh"
#include "ast/IntegerLiteral.hh"
#include "ast/Operator.hh"
#include "ast/StringLiteral.hh"
#include "ast/VariableExpression.hh"

TokenBuffer Parser::shunting_yard(TokenBuffer& tokens) {
    std::stack<Token> operators {};
    std::vector<Token> output {};

    int par_depth = 0;

    while (!tokens.empty()) {
        if (tokens.top().type == SEMICOLON) break;
        if (tokens.top().type == COMMA) break;
        if (tokens.top().type == RBRACKET) break;
        if (tokens.top().type == RPARENS && par_depth == 0) break;

        Token current { tokens.pop() };

        if (current.type == NUMBER || current.type == STRING) {
            output.push_back(current);
        } else if (current.type == IDENTIFIER) {
            if (tokens.top().type == LPARENS) {  // Call to function

                current.type = FUNC_CALL;
                output.push_back(current);  // Function call ident

                int current_par_depth = par_depth;

                do {
                    if (tokens.top().type == LPARENS) ++current_par_depth;
                    if (tokens.top().type == RPARENS) --current_par_depth;
                    output.push_back(tokens.pop());
                } while (current_par_depth > par_depth);

            } else {  // Variable
                output.push_back(current);
            }
        } else if (Operator::is_operator(current)) {
            Operator current_operator { current };

            while (!operators.empty() &&
                   Operator::is_operator(operators.top())) {
                Token operator_top = operators.top();
                Operator other_operator { operator_top };

                if ((current_operator.is_left_associative() &&
                     current_operator.precedence > other_operator.precedence) ||
                    (current_operator.is_right_associative() &&
                     current_operator.precedence <=
                         other_operator.precedence)) {
                    break;
                }

                output.push_back(operator_top);
                operators.pop();
            }

            operators.push(current);

        } else if (current.type == LBRACKET) {
            int bracket_depth { 1 };
            output.push_back(current);

            do {
                if (tokens.top().type == LBRACKET) ++bracket_depth;
                if (tokens.top().type == RBRACKET) --bracket_depth;
                output.push_back(tokens.pop());
            } while (bracket_depth > 0);

        } else if (current.type == LPARENS) {
            ++par_depth;
            operators.push(current);
        } else if (current.type == RPARENS) {
            --par_depth;
            while (operators.top().type != LPARENS) {
                output.push_back(operators.top());
                operators.pop();
            }

            operators.pop();  // Pop parens
            // @TODO: Handle mismatched parentheses
        } else {
            throw std::runtime_error("Invalid expression token");
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    return TokenBuffer { output };
}

ptr_t<Expression> Parser::rpn_to_expressions(TokenBuffer& tokens) {
    std::stack<ptr_t<Expression>> expressions {};

    while (!tokens.empty()) {
        Token current = tokens.pop();

        if (current.type == NUMBER) {
            expressions.push(std::make_unique<IntegerLiteral>(current));
        } else if (current.type == STRING) {
            expressions.push(std::make_unique<StringLiteral>(current));
        } else if (current.type == FUNC_CALL) {
            ptr_t<ArgumentList> argument_list =
                Parser::parse_argument_list(tokens);
            expressions.push(
                std::make_unique<FunctionCall>(current, argument_list));

        } else if (current.type == IDENTIFIER) {  // Variable
            // @TODO: We need to add a source_ref to var_expr
            expressions.push(std::make_unique<VariableExpression>(current));
        } else if (Operator::is_operator(current)) {
            if (expressions.size() < 2) {
                throw std::runtime_error("Two operands needed");
            }

            ptr_t<Expression> right = std::move(expressions.top());
            expressions.pop();

            ptr_t<Expression> left = std::move(expressions.top());
            expressions.pop();

            ptr_t<Operator> op { std::make_unique<Operator>(current) };

            expressions.push(
                std::make_unique<BinaryExpression>(right, left, op));

        } else if (current.type == LBRACKET) {
            // @REFACTOR: This shouldn't be needed. This isn't in line with how
            // other things are parsed here.
            tokens.backtrack(1);
            expressions.push(parse_array_literal(tokens));
        } else {
            error::rpn(current);
        }
    }

    if (expressions.size() != 1) {
        error::syntax("Invalid expression", tokens);
    }

    return std::move(expressions.top());
}

ptr_t<Expression> Parser::parse_expression(TokenBuffer& tokens) {
    // @TODO/BUG: This is simply not working. A new TokenBuffer is created with
    //            each call to shunting_yard, which will create invalid
    //            iterators for the SourceRef struct.

    auto begin = TokenBuffer::get_program_buffer()->top_iterator();
    TokenBuffer rpn_tokens = shunting_yard(tokens);
    auto end = TokenBuffer::get_program_buffer()->top_iterator();
    ptr_t<Expression> expression = rpn_to_expressions(rpn_tokens);
    expression->source_ref.begin = begin;
    expression->source_ref.end = end;
    expression->source_ref.size = TokenBuffer::get_program_buffer()->size();
    std::cout << expression->dump(0) << std::endl;
    return expression;
}
