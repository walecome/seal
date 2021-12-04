#include <utility>

#include "Error.hh"
#include "ast/Type.hh"

void error::syntax_error(const std::string_view err) { report_error(err); }

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

void error::report_error(const std::string_view err, bool quit) {
    std::cout << Color::Modifier(Color::FG_RED) << err
              << Color::Modifier(Color::FG_DEFAULT) << std::endl;

    if (quit) exit(EXIT_FAILURE);
}

std::string line_text(SourceRef source_ref) {
    std::ostringstream oss {};

    oss << "Line " << (source_ref.begin->row + 1) << " column "
        << (source_ref.begin->col + 1);

    return oss.str();
}

void error::mismatched_type(const Type &a, const Type &b,
                            SourceRef source_ref) {
    std::ostringstream oss {};

    auto tokens = TokenBuffer::source_tokens(source_ref);

    oss << line_text(source_ref) << ": ";
    oss << "Mismatched types, got " << a.to_user_string() << " and " << b.to_user_string()
        << std::endl;
    oss << "\t" << tokens.as_source();
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

void error::add_semantic_error(const std::string error_prefix,
                               SourceRef source_ref) {
    auto tokens = TokenBuffer::source_tokens(source_ref);

    std::ostringstream oss {};
    oss << line_text(source_ref) << ": ";
    oss << error_prefix << std::endl;
    oss << "\t" << tokens.as_source();

    add_semantic_error(oss.str());
}

void error::report_semantic_errors() {
    if (semantic_errors.empty()) return;
    for (auto &error : semantic_errors) {
        report_error(error, false);
    }
    exit(EXIT_FAILURE);
}
