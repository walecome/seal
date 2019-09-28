#include "Parser.hh"

TokenBuffer Parser::shuntingYard(TokenBuffer& tokens) {
    std::stack<Token> operators {};
    std::vector<Token> output {};

    int parDepth = 0;

    while (!tokens.empty()) {
        if (tokens.top().type == SEMICOLON) break;
        if (tokens.top().type == COMMA) break;
        if (tokens.top().type == RPARENS && parDepth == 0) break;
        Token current { tokens.pop() };

        if (current.type == NUMBER) {
            output.push_back(current);
        } else if (current.type == IDENTIFIER) {
            // @ TODO: Push function to operator stack...
            if (tokens.top().type == LPARENS) {  // Call to function
                current.type = FUNC_CALL;
                output.push_back(current);
                while (tokens.top().type != RPARENS) {
                    output.push_back(tokens.pop());
                }
                output.push_back(tokens.pop());  // RPARENS
            } else {                             // Variable
                operators.push(current);
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

        } else if (current.type == LPARENS) {
            ++parDepth;
            operators.push(current);
        } else if (current.type == RPARENS) {
            --parDepth;
            while (operators.top().type != LPARENS) {
                output.push_back(operators.top());
                operators.pop();
            }
            // @TODO: Handle mismatched parentheses
        }
    }

    while (!operators.empty()) {
        output.push_back(operators.top());
        operators.pop();
    }

    for (auto&& x : output) {
        std::cout << x.toString() << std::endl;
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

        } else {
            Error::rpn(current);
        }
    }
    return std::move(expressions.top());
}

ptr_t<Expression> Parser::parseExpression(TokenBuffer& tokens) {
    TokenBuffer rpnTokens = shuntingYard(tokens);
    ptr_t<Expression> expression = rpnToExpressions(rpnTokens);
    std::cout << expression->dump() << std::endl;

    return expression;
}
