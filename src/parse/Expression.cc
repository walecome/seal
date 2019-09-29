#include "Parser.hh"

TokenBuffer Parser::shuntingYard(TokenBuffer& tokens) {
    std::stack<Token> operators {};
    std::vector<Token> output {};

    int parDepth = 0;

    while (!tokens.empty()) {
        // @TODO: Change from eat() to top().type
        if (tokens.eat(SEMICOLON)) break;
        if (tokens.eat(COMMA)) break;
        if (tokens.top().type == RBRACKET) break;
        if (tokens.top().type == RPARENS && parDepth == 0) break;

        Token current { tokens.pop() };

        if (current.type == NUMBER || current.type == STRING) {
            output.push_back(current);
        } else if (current.type == IDENTIFIER) {
            if (tokens.top().type == LPARENS) {  // Call to function

                current.type = FUNC_CALL;
                output.push_back(current);  // Function call ident

                int currentParDepth = parDepth;

                do {
                    if (tokens.top().type == LPARENS) ++currentParDepth;
                    if (tokens.top().type == RPARENS) --currentParDepth;
                    output.push_back(tokens.pop());
                } while (currentParDepth > parDepth);

            } else {  // Variable
                output.push_back(current);
            }
        } else if (Operator::isOperator(current)) {
            Operator currentOperator { current };

            while (!operators.empty() &&
                   Operator::isOperator(operators.top())) {
                Token operatorTop = operators.top();
                Operator otherOperator { operatorTop };

                if ((currentOperator.isLeftAssociative() &&
                     currentOperator.precedence > otherOperator.precedence) ||
                    (currentOperator.isRightAssociative() &&
                     currentOperator.precedence <= otherOperator.precedence)) {
                    break;
                }

                output.push_back(operatorTop);
                operators.pop();
            }

            operators.push(current);

        } else if (current.type == LBRACKET) {
            int bracketDepth { 1 };
            output.push_back(current);

            do {
                if (tokens.top().type == LBRACKET) ++bracketDepth;
                if (tokens.top().type == RBRACKET) --bracketDepth;
                output.push_back(tokens.pop());
            } while (bracketDepth > 0);

        } else if (current.type == LPARENS) {
            ++parDepth;
            operators.push(current);
        } else if (current.type == RPARENS) {
            --parDepth;
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

ptr_t<Expression> Parser::rpnToExpressions(TokenBuffer& tokens) {
    std::stack<ptr_t<Expression>> expressions {};

    while (!tokens.empty()) {
        Token current = tokens.pop();

        if (current.type == NUMBER) {
            int number = std::stoi(current.value);
            expressions.push(std::make_unique<IntegerLiteral>(number));
        } else if (current.type == STRING) {
            expressions.push(std::make_unique<StringLiteral>(current.value));
        } else if (current.type == FUNC_CALL) {
            ptr_t<ArgumentList> argumentList =
                Parser::parseArgumentList(tokens);
            expressions.push(
                std::make_unique<FunctionCall>(current.value, argumentList));

        } else if (current.type == IDENTIFIER) {  // Variable
            expressions.push(
                std::make_unique<VariableExpression>(current.value));
        } else if (Operator::isOperator(current)) {
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
            expressions.push(parseArrayLiteral(tokens));
        } else {
            Error::rpn(current);
        }
    }

    if (expressions.size() != 1) {
        Error::syntax("Invalid expression", tokens);
    }

    return std::move(expressions.top());
}

ptr_t<Expression> Parser::parseExpression(TokenBuffer& tokens) {
    TokenBuffer rpnTokens = shuntingYard(tokens);
    ptr_t<Expression> expression = rpnToExpressions(rpnTokens);
    return expression;
}
