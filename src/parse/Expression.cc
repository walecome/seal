#include "Parser.hh"

std::vector<Token> Parser::shuntingYard(TokenBuffer& tokens) {
    std::stack<Token> operators {};
    std::vector<Token> output {};

    while (tokens.top().type != SEMICOLON) {
        Token current { tokens.pop() };

        if (current.type == NUMBER) {
            output.push_back(current);
        } else if (current.type == IDENTIFIER) {
            // @ TODO: Push function to operator stack...
            if (tokens.top().type == LPARENS) {  // Call to function
                current.type = FUNC_CALL;
                while (tokens.top().type != RPARENS) {
                    operators.push(tokens.pop());
                }
                operators.push(tokens.pop());
            } else {  // Variable
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
            operators.push(current);
        } else if (current.type == RPARENS) {
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

    return output;
}

ptr_t<Expression> Parser::rpnToExpressions(TokenBuffer& tokens) {
    while (!tokens.empty()) {
        Token current = tokens.pop();

        if (current.type == NUMBER) {
        } else if (current.type == FUNC_CALL) {
        } else if (Operator::isOperator(current)) {
        } else {
            std::ostringstream os {};
            os << "Invalid token when converting RPN to expression, expected "
               << tokenNames[current.type];
            os << " got " << tokenNames[current.type];
            throw std::runtime_error { os.str() };
        }
    }
}

ptr_t<Expression> Parser::parseExpression(TokenBuffer& tokens) {
    std::vector<Token> rpnTokens { shuntingYard(tokens) };
}
