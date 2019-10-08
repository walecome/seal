#include "Error.hh"
#include "ast/Type.hh"

void error::syntax(TokenType expected, Token got) {
    std::ostringstream oss {};
    oss << "Invalid syntax, expected " << token_names[expected];
    oss << " got " << token_names[got.type];
    report_error(oss.str());
}

void error::syntax(const std::string &s, TokenBuffer &tokens) {
    std::ostringstream oss {};
    oss << s << " " << tokens.dump() << std::endl;
    report_error(oss.str());
}

void error::rpn(Token got) {
    std::ostringstream oss {};
    oss << "Invalid token when converting RPN to expression, "
        << " got " << token_names[got.type];
    report_error(oss.str());
}

void error::report_error(const std::string &error, bool quit) {
    std::cout << Color::Modifier(Color::FG_RED) << "ERROR: " << error
              << Color::Modifier(Color::FG_DEFAULT) << std::endl;

    if (quit) exit(EXIT_FAILURE);
}

void error::mismatched_type(const Type &expected, const Type &found,
                            const Token &token) {
    std::ostringstream oss {};
    oss << "Mismatched types, expected " << expected.to_string() << " found "
        << found.to_string() << " " << token.to_string();
    add_semantic_error(oss.str());
}

void error::add_semantic_error(const std::string error) {
    semantic_errors.push_back(error);
}

void error::add_semantic_error(const std::string error_prefix,
                               const Token &token) {
    std::ostringstream oss {};
    oss << error_prefix << ": " << token.to_string();
    add_semantic_error(oss.str());
}

void error::report_semantic_errors() {
    if (semantic_errors.empty()) return;
    for (auto &error : semantic_errors) {
        report_error(error, false);
    }
    exit(EXIT_FAILURE);
}