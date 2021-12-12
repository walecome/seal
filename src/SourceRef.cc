#include "SourceRef.hh"
#include "ast/Node.hh"

SourceRefGuard::SourceRefGuard(TokenBuffer& tokens) : m_begin(tokens.top_iterator()), m_tokens(&tokens) { }

void SourceRefGuard::finalize(Node* target) const {
    target->source_ref.begin = m_begin;
    target->source_ref.end = m_tokens->top_iterator();
}

SourceRef SourceRefGuard::now() const {
    SourceRef ret;
    ret.begin = m_begin;
    ret.end = m_tokens->top_iterator();
    return ret;
}
