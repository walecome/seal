#pragma once
#include "TokenBuffer.hh"

/*
    Reference to the source file tokens. 'end' member is like std::end, it is
    supposed to point to the iterator next outside the reference scope.
*/
struct SourceRef {
    TokenBuffer::Iterator begin;
    TokenBuffer::Iterator end;

    int size { -1 };
};

class Node;

class SourceRefGuard {
    public:
        SourceRefGuard(TokenBuffer& tokens);
        void finalize(Node* target) const;
        SourceRef now() const;

    private:
        const TokenBuffer::Iterator m_begin;
        TokenBuffer* m_tokens;
};
