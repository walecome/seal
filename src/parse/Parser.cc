#include "Parser.hh"

void Parser::parse(TokenBuffer &tokens) {
    while (!tokens.empty()) {
        std::cout << "Parsing function" << std::endl;
        ptr_t<Function> function = parseFunction(tokens);

        if (!function) break;

        parseNodes.push_back(std::move(function));
    };
}
